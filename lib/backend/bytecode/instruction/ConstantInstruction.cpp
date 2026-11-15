#include "ConstantInstruction.hpp"

ConstantInstruction::ConstantInstruction(const Value& value)
: value_(value) { }

void ConstantInstruction::Execute(IExecutionContext& context) const {
    context.PushToStack(value_);
}
