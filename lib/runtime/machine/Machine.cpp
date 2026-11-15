#include "Machine.hpp"

#include <utility>

#include "ExecutionContext.hpp"

Machine::Machine(IValueStorePtr variables, IInputPtr input, IOutputPtr output)
: variables_(std::move(variables))
, input_(std::move(input))
, output_(std::move(output)) { }

void Machine::Execute(const Program& program) const {
    ExecutionContext context(program, variables_, input_, output_);

    while (context.HasNext()) {
        context.Next().Execute(context);
    }
}
