#pragma once

#include "binary_operator/IBinaryOperator.hpp"

template <class Operator>
class Swapped final : public IBinaryOperator {
public:
    Value Apply(const Value& lhs, const Value& rhs) const override {
        return operator_.Apply(rhs, lhs);
    }

private:
    Operator operator_;
};