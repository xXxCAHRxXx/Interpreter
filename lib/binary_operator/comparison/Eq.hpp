#pragma once

#include "binary_operator/IBinaryOperator.hpp"

class Eq final : public IBinaryOperator {
public:
    Value Apply(const Value& lhs, const Value& rhs) const override {
        return Value(lhs == rhs);
    }
};