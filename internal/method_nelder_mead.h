#ifndef NEDLER_MEAD_NELDER_MEAD_H
#define NEDLER_MEAD_NELDER_MEAD_H

#include <optional>

#include "common.h"
#include "method.h"

struct NelderMeadDebugInfo {
    bool debug = false;
    size_t call_counter = 0;

    void log(const std::vector<Point>&curr, const std::vector<Point>&prev, const double mse) {
        if (debug) {
            call_counter += 1;
            std::cout << "#" << call_counter << "\t" << prev << "\t->\t" << curr << "\tMSE: " << mse << std::endl;
        }
    }
};


class NelderMeadMethod final : public Method {
    std::optional<std::vector<Point>> start_;
    double tolerance_;
    double alpha_;
    double gamma_;
    double rho_;
    double sigma_;

    // https://en.wikipedia.org/wiki/Nelder–Mead_method
    // alpha > 0
    // gamma > 1
    // 0 < rho <= 0.5
    PointValue minimal_internal(const Function&func, std::vector<Point>&x) const;

public:
    // If NedlerMeadMethod's (start == None) => (it's chosen randomly each run)
    explicit NelderMeadMethod(Tracer tracer = Tracer::muted(),
                              const double tolerance = 1,
                              std::optional<std::vector<Point>> start = {},
                              const double alpha = 1,
                              const double gamma = 2,
                              const double rho = 0.5,
                              const double sigma = 0.5)
        : Method(std::move(tracer)),
          start_(std::move(start)),
          tolerance_(tolerance),
          alpha_(alpha),
          gamma_(gamma),
          rho_(rho),
          sigma_(sigma) {
    }

    NelderMeadMethod& with(std::vector<Point> start) {
        start_ = std::move(start);
        return *this;
    }

    [[nodiscard]] std::string name() const override { return "Nelder Mead method"; }

    [[nodiscard]] PointValue minimal(const Function&func, const Area&where) const override {
        auto x = std::vector<Point>{};
        if (start_.has_value()) {
            x = start_.value();
        }
        else {
            x = where.border_vertexes();
        }

        return minimal_internal(func, x);
    }
};


#endif //NEDLER_MEAD_NELDER_MEAD_H
