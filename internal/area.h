#ifndef AREA_H
#define AREA_H


#include <sstream>
#include <utility>

#include "point.h"

class Area {
    Point min_, max_;

public:
    static Area cube(const size_t dimensions, const double min, const double max) {
        return Area(Point(std::vector(dimensions, min)), Point(std::vector(dimensions, max)));
    }

    Area(Point min, Point max) : min_(std::move(min)), max_(std::move(max)) {
        if (min_.size() != max_.size()) {
            throw std::invalid_argument("min_.size() != max_.size()");
        }
        if (min_.size() == 0) {
            throw std::invalid_argument("min_.size() == 0");
        }
    }

    [[nodiscard]] bool contains(const Point&point) const {
        if (point.size() != dimensions()) {
            throw std::invalid_argument("the point is from other dimestion");
        }

        for (size_t i = 0; i < point.size(); i++) {
            if (min_[i] > point[i] || max_[i] < point[i]) {
                return false;
            }
        }

        return true;
    }

    [[nodiscard]] Point random_point() const {
        return Point::random(min_.size(), min_, max_);
    }

    [[nodiscard]] std::vector<Point> border_vertexes() const {
        auto points = std::vector{min_, max_};
        for (size_t i = 1; i < dimensions(); i++) {
            auto next = std::vector<Point>{};
            for (auto&point: points) {
                next.push_back(point.extended(min_[i]));
                next.push_back(point.extended(max_[i]));
            }
            points = std::move(next);
        }
        return points;
    }

    [[nodiscard]] size_t dimensions() const {
        return min_.size();
    }

    [[nodiscard]] std::string to_string() const {
        std::ostringstream oss;
        for (size_t i = 0; i < dimensions(); i++) {
            if (i != 0) {
                oss << " x ";
            }
            oss << '[' << min_[i] << ", " << max_[i] << ']';
        }
        return oss.str();
    }
};


#endif //AREA_H
