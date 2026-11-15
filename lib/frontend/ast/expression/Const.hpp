#pragma once

#include "IExpr.hpp"
#include "value/Value.hpp"

class Const : public IExpr {
public:
    explicit Const(const Value& value);

    const Value& GetValue() const noexcept;

    void Accept(IExprVisitor& visitor) const override;

private:
    Value value_;
};
