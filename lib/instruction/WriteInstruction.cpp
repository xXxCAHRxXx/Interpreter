#include "WriteInstruction.hpp"

void WriteInstruction::Execute(IExecutionContext& context) const {
    context.WriteOutput(context.PopFromStack());
}
