#include "ReadInstruction.hpp"

void ReadInstruction::Execute(IExecutionContext& context) const {
    context.PushToStack(context.ReadInput());
}
