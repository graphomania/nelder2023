#ifndef TEST_FUNCTIONS_H
#define TEST_FUNCTIONS_H

#include "point.h"

// https://en.wikipedia.org/wiki/Himmelblau%27s_function
// It has one local maximum at M = {0.270845, 0.923039}, func(M) = 181.617
//
// Four identical local minima:
// 1. func({ 3,         2})        = 0
// 2. func({-2.805118,  3.131312}) = 0
// 3. func({-3.779310, -3.283186}) = 0
// 4. func({ 3.584428, -1.848126}) = 0
double himmelblau_function(const Point&p) {
    if (p.size() != 2) {
        std::cerr << "WARNING himmelblau_function: point.dimension != 2 (" << p.size() << " != 2)\n";
    }
    return sqr(sqr(p[0]) + p[1] - 11) + sqr(p[0] + sqr(p[1]) - 7);
}


// https://en.wikipedia.org/wiki/Test_functions_for_optimization
double rastrigin_function(const Point&p) {
    constexpr double A = 10;

    double ret = A * p.size();
    for (auto x: p) {
        ret += sqr(x) - A * std::cos(2 * std::numbers::pi * x);
    }
    return ret;
}

#endif //TEST_FUNCTIONS_H
