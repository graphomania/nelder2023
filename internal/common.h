#ifndef NEDLER_MEAD_UTILITY_H
#define NEDLER_MEAD_UTILITY_H

#include "point.h"
#include "area.h"

// Function: R^n --> R
using Function = std::function<double(const Point&)>;

using std::move;

inline double MSE_with_func_as_extra_coordinate(const Function&func,
                                                const std::vector<Point>&lhs,
                                                const std::vector<Point>&rhs) {
    assert(lhs.size() == rhs.size());

    double ret = 0;
    for (size_t i = 0; i < lhs.size(); i++) {
        ret += sqr(lhs[i].extended(func(lhs[i])).distance(rhs[i].extended(func(rhs[i])))) / lhs.size();
    }
    return ret;
}

template<typename T>
std::vector<T> sub_vector(const std::vector<T>&vec, size_t begin, size_t end) {
    return {vec.begin() + begin, vec.end() - (vec.size() - end)};
}

inline Point centroid(const std::vector<Point>&polygon) {
    auto ret = polygon[0];
    for (size_t i = 1; i < polygon.size(); i++) {
        ret = ret + polygon[i];
    }
    return ret / polygon.size();
}

auto abs(auto val) {
    if (val < 0) {
        return -val;
    }
    return val;
}

namespace functional {
    inline Function invert(const Function&func) {
        return [=](const Point&point) {
            return func(point);
        };
    }

    template<typename T>
    T min(const T&lhs, const T&rhs, std::function<bool(const T&, const T&)> cmp) {
        if (cmp(lhs, rhs)) {
            return lhs;
        }
        return rhs;
    }
}


#endif //NEDLER_MEAD_UTILITY_H
