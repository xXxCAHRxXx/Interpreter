#include "JumpNotZeroInstruction.hpp"

#include <utility>

JumpNotZeroInstruction::JumpNotZeroInstruction(Label label)
: label_(std::move(label)) { }

void JumpNotZeroInstruction::Execute(IExecutionContext& context) const {
    if (context.PopFromStack()) {
        context.Jump(label_);
    }
}
