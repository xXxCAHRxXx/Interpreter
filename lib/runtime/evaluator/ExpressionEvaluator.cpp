#include "ExpressionEvaluator.hpp"

#include "ast/expression/IExpr.hpp"
#include "error/LogicError.hpp"
#include "ast/expression/Var.hpp"
#include "ast/expression/Const.hpp"
#include "ast/expression/BinOp.hpp"

ExpressionEvaluator::ExpressionEvaluator(IValueReaderPtr values) 
: values_(std::move(values))
{ }

Value ExpressionEvaluator::Evaluate(IExprPtr expr) {
    expr->Accept(*this);

    return result_;
}

void ExpressionEvaluator::Visit(const Var& node) {
    result_ = values_->Get(node.GetName());
}

void ExpressionEvaluator::Visit(const Const& node) {
    result_ = node.GetValue();
}

void ExpressionEvaluator::Visit(const BinOp& node) {
    Value lhs = Evaluate(node.GetLhs());
    Value rhs = Evaluate(node.GetRhs());
    result_ = node.GetOperator()->Apply(lhs, rhs);
}