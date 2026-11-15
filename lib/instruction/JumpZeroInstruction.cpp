#include "JumpZeroInstruction.hpp"

#include <utility>

JumpZeroInstruction::JumpZeroInstruction(Label label)
: label_(std::move(label)) { }

void JumpZeroInstruction::Execute(IExecutionContext& context) const {
    if (!context.PopFromStack()) {
        context.Jump(label_);
    }
}
