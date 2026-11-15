#include "LoadInstruction.hpp"

#include <utility>

LoadInstruction::LoadInstruction(std::string name)
: name_(std::move(name)) { }

void LoadInstruction::Execute(IExecutionContext& context) const {
    context.PushToStack(context.LoadVariable(name_));
}
