#ifndef RANDOM_WALK_H
#define RANDOM_WALK_H

#include <optional>
#include <utility>

#include "common.h"
#include "trace.h"
#include "method.h"


class RandomWalk final : public Method {
    size_t min_, max_;
    double tolerance_;

public:
    explicit RandomWalk(Tracer tracer = Tracer::muted(), const double tolerance = 1e-5,
                        const size_t min = 8, const size_t max = 16)
        : Method(std::move(tracer)),
          min_(min),
          max_(max),
          tolerance_(tolerance) {
    }

    [[nodiscard]] std::string name() const override { return "random walk method"; }

    [[nodiscard]] PointValue minimal(const Function&func, const Area&where) const override {
        std::optional<PointValue> min;
        for (size_t iter = 1; min_ > iter || iter <= max_; iter++) {
            auto point = where.random_point();
            const auto value = func(point);
            if (!min.has_value()) {
                min = {std::move(point), value};
                tracer_.trace_numbered(min->first, min->second);
                continue;
            }

            if (abs(min.value().second - value) < tolerance_) {
                return min.value();
            }

            if (value < min.value().second) {
                min = {point, value};
                tracer_.trace_numbered(min->first, min->second);
            }
        }
        return min.value();
    }
};

#endif //RANDOM_WALK_H
