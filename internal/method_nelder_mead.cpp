#include "method_nelder_mead.h"

using namespace std;

PointValue NelderMeadMethod::minimal_internal(const Function&func, vector<Point>&x) const {
    auto return_or_go_deeper = [&, prev_x = x]() -> PointValue {
        const auto mse = MSE_with_func_as_extra_coordinate(func, x, prev_x);
        if (mse < tolerance_) {
            return {x[0], func(x[0])};
        }
        // tracer_.trace()
        tracer_.trace_polygon(x, prev_x, mse);
        return minimal_internal(func, x);
    };

    // 1. Order
    ranges::sort(x, [&](const auto&lhs, const auto&rhs) -> bool {
        return func(lhs) < func(rhs);
    });

    // 2. Calculate x_o, the centroid of all points except x_n+1
    const auto x_o = centroid(sub_vector(x, 0, x.size() - 1));

    // 3. Reflection
    const auto x_r = x_o + (x_o - x.back()) * alpha_;
    if (func(x[0]) <= func(x_r) && func(x_r) < func(x[x.size() - 2])) {
        x.back() = x_r;
        return return_or_go_deeper();
    }

    // 4. Expansion
    if (func(x_r) < func(x[0])) {
        if (const auto x_e = x_o + (x_r - x_o) * gamma_; func(x_e) < func(x_r)) {
            x.back() = x_e;
            return return_or_go_deeper();;
        }

        x.back() = x_r;
        return return_or_go_deeper();
    }

    // 5. Contraction
    if (func(x_r) >= func(x[x.size() - 2])) {
        if (func(x_r) < func(x.back())) {
            if (const auto x_c = x_o + (x_r - x_o) * rho_; func(x_c) < func(x_r)) {
                x.back() = x_c;
                return return_or_go_deeper();
            }
        }
        else {
            if (const auto x_c = x_o + (x.back() - x_o) * rho_; func(x_c) < func(x_r)) {
                x.back() = x_c;
                return return_or_go_deeper();
            }
        }
    }

    // 6. Shrink
    for (size_t i = 1; i < x.size(); i++) {
        x[i] = x[0] + (x[i] - x[0]) * sigma_;
    }
    return return_or_go_deeper();
}
