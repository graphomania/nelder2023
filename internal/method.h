#ifndef METHOD_H
#define METHOD_H

#include "trace.h"

class Method {
protected:
    Tracer tracer_;

public:
    virtual ~Method() = default;

    explicit Method(Tracer tracer) : tracer_(std::move(tracer)) {
    }

    virtual std::string name() const {
        return "unnamed method";
    }

    virtual PointValue minimal(const Function&func, const Area&where) const = 0;

    virtual PointValue maximal(const Function&func, const Area&where) const {
        return minimal(functional::invert(func), where);
    }
};

#endif //METHOD_H
