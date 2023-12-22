#ifndef POINT_H
#define POINT_H

#include <iostream>
#include <algorithm>
#include <cassert>
#include <vector>
#include <exception>
#include <functional>
#include <random>


class Point;

using PointValue = std::pair<Point, double>;

auto sqr(const auto&x) {
    return x * x;
}

class Point : public std::vector<double> {
    void assert_match_sizes(const Point&other) const {
        if (this->size() != other.size())
            throw std::invalid_argument("sizes dont match");
    }

public:
    [[nodiscard]] double distance(const Point&other) const {
        assert_match_sizes(other);

        double ret = 0;
        for (size_t i = 0; i < size(); i++)
            ret += sqrt(sqr(at(i) - other.at(i)));
        return ret;
    }

    Point operator+(const Point&other) const {
        assert_match_sizes(other);

        auto ret = Point{};
        ret.resize(size());

        for (size_t i = 0; i < size(); i++) {
            ret[i] = at(i) + other.at(i);
        }

        return ret;
    }

    Point operator-(const Point&other) const {
        return *this + other * (-1);
    }

    Point operator*(const double x) const {
        auto ret = Point{};
        ret.resize(size());
        for (size_t i = 0; i < size(); i++) {
            ret[i] = at(i) * x;
        }
        return ret;
    }

    Point operator/(const double x) const {
        return *this * (1.0 / x);
    }

    [[nodiscard]] Point extended(const double x) const {
        auto copy = *this;
        copy.push_back(x);
        return copy;
    }

    static Point random(const size_t dimension, const double min, const double max) {
        static std::default_random_engine re;

        auto ret = Point{};
        ret.resize(dimension);
        std::uniform_real_distribution unif(min, max);
        for (double&i: ret) {
            i = unif(re);
        }
        return ret;
    }

    static Point random(const size_t dimension, const Point&min, const Point&max) {
        static std::default_random_engine re;

        if (dimension != min.size() || min.size() != max.size()) {
            throw std::invalid_argument("dimension == min.size() == max.size() is required");
        }

        auto ret = Point{};
        ret.resize(dimension);
        for (size_t i = 0; i < dimension; i++) {
            std::uniform_real_distribution unif(min[i], max[i]);
            ret[i] = unif(re);
        }
        return ret;
    }

    static vector<Point> generate_vector(const size_t size, const size_t dimensions,
                                         const double min, const double max) {
        auto ret = vector<Point>(size);
        for (size_t i = 0; i < size; i++) {
            ret[i] = Point::random(dimensions, min, max);
        }
        return ret;
    }

    std::ostream& operator<<(std::ostream&out) const {
        out << "{";
        for (size_t i = 0; i < size(); i++) {
            if (i != 0) {
                std::cout << ", ";
            }
            std::cout << at(i);
        }
        return out << "}";
    }
};

template<typename T>
std::ostream& operator<<(std::ostream&out, const std::vector<T>&vec) {
    out << "[";
    for (size_t i = 0; i < vec.size(); i++) {
        if (i != 0) {
            out << ", ";
        }
        out << vec.at(i);
    }
    return out << "]";
}

#endif //POINT_H
