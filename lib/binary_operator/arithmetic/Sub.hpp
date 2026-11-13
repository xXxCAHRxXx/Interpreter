#pragma once

#include "binary_operator/IBinaryOperator.hpp"

class Sub final : public IBinaryOperator {
public:
    Value Apply(const Value& lhs, const Value& rhs) const override {
        return lhs - rhs;
    }
};
