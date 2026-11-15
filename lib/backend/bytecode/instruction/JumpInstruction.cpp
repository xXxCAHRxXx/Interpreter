#include "JumpInstruction.hpp"

#include <utility>

JumpInstruction::JumpInstruction(Label label)
: label_(std::move(label)) { }

void JumpInstruction::Execute(IExecutionContext& context) const {
    context.Jump(label_);
}
