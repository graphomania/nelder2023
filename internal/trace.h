#ifndef LOG_H
#define LOG_H

#include "point.h"

#include <iomanip>
#include <sstream>

namespace format {
    static std::string time() {
        const auto t = std::time(nullptr);
        const auto tm = *std::localtime(&t);
        std::ostringstream oss;
        oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
        return oss.str();
    }

    static void log(const std::string&what) {
        std::cerr << format::time() << "\t" << what << '\n';
    }

    static void pass(const std::string&what) {
    }
};

class Tracer {
    std::function<void(const std::string&)> log_function = format::log;
    std::string prefix_;
    mutable size_t n_ = 0;

public:
    static Tracer muted() {
        return Tracer(format::pass);
    }

    static Tracer logging() {
        return Tracer(format::log);
    }

    explicit Tracer(auto logger) : log_function(logger) {
    }

    explicit Tracer(const bool muted) : log_function{} {
        if (muted) {
            log_function = format::pass;
        }
    }

    void trace(const Point&point, const double value) const {
        std::ostringstream oss;
        oss << prefix_ << point << " -> " << value;
        log_function(oss.str());
    }

    void trace_polygon(const std::vector<Point>&curr, const std::vector<Point>&prev,
                       const std::optional<double> mse = {}) const {
        std::ostringstream oss;
        oss << "#" << ++n_ << "\t" << prev << "\t->\t" << curr;
        if (mse.has_value()) {
            oss << "\tMSE: " << mse.value();
        }
        log_function(oss.str());
    }

    void trace_numbered(const Point&point, const double value) const {
        std::ostringstream oss;
        oss << prefix_ << ++n_ << '.' << '\t' << point << " -> " << value;
        log_function(oss.str());
    }


    Tracer& prefix(std::string prefix) {
        prefix_ = std::move(prefix);
        return *this;
    }
};

#endif //LOG_H
