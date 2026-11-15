#include "StoreInstruction.hpp"

#include <utility>

StoreInstruction::StoreInstruction(std::string name)
: name_(std::move(name)) { }

void StoreInstruction::Execute(IExecutionContext& context) const {
    context.StoreVariable(name_, context.PopFromStack());
}
