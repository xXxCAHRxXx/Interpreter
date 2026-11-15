#include "BinaryOperationInstruction.hpp"

#include <utility>

BinaryOperationInstruction::BinaryOperationInstruction(IBinaryOperatorPtr op)
: operator_(std::move(op)) { }

void BinaryOperationInstruction::Execute(IExecutionContext& context) const {
    const Value rhs = context.PopFromStack();
    const Value lhs = context.PopFromStack();

    context.PushToStack(operator_->Apply(lhs, rhs));
}
