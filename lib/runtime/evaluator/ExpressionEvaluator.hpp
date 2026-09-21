#pragma once

#include <gsl/pointers>

#include "ast/expression/visitor/IExprVisitor.hpp"
#include "IExpressionEvaluator.hpp"
#include "value/Value.hpp"
#include "runtime/environment/IValueReader.hpp"
#include "ast/expression/IExpr.hpp"

class ExpressionEvaluator final : public IExprVisitor, public IExpressionEvaluator  {
public:
    explicit ExpressionEvaluator(IValueReaderPtr values);

    Value Evaluate(IExprPtr expr);

    void Visit(const Var& node) override;
    void Visit(const Const& node) override;
    void Visit(const BinOp& node) override;
private:
    IValueReaderPtr values_;
    Value result_;
};
