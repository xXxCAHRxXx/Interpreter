#include "BytecodeInterpreterFactory.hpp"

#include <memory>
#include <utility>

#include "backend/bytecode/compiler/Compiler.hpp"
#include "backend/runtime/environment/Environment.hpp"
#include "interpreter/BytecodeInterpreter.hpp"
#include "backend/bytecode/machine/Machine.hpp"

BytecodeInterpreterFactory::BytecodeInterpreterFactory(IParserPtr parser, IInputPtr input, IOutputPtr output)
: parser_(std::move(parser))
, input_(std::move(input))
, output_(std::move(output)) { }

IInterpreterPtr BytecodeInterpreterFactory::Create() const {
    auto values = std::make_shared<Environment>();
    auto machine = std::make_shared<Machine>(values, input_, output_);

    return std::make_shared<BytecodeInterpreter>(parser_, std::make_shared<Compiler>(), machine);
}
