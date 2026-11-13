#pragma once

#include "binary_operator/IBinaryOperator.hpp"

class Or final : public IBinaryOperator {
public:
    Value Apply(const Value& lhs, const Value& rhs) const override {
        return Value(static_cast<bool>(lhs) || static_cast<bool>(rhs));
    }
};
