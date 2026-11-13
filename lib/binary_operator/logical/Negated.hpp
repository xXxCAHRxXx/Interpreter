#pragma once

#include "binary_operator/IBinaryOperator.hpp"

template <class Operator>
class Negated final : public IBinaryOperator {
public:
    Value Apply(const Value& lhs, const Value& rhs) const override {
        return Value(!static_cast<bool>(operator_.Apply(lhs, rhs)));
    }

private:
    Operator operator_;
};
