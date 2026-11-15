#include "BytecodeInterpreterFactory.hpp"

#include <memory>
#include <utility>

#include "compiler/Compiler.hpp"
#include "runtime/environment/Environment.hpp"
#include "runtime/interpreter/BytecodeInterpreter.hpp"
#include "runtime/machine/Machine.hpp"

BytecodeInterpreterFactory::BytecodeInterpreterFactory(IParserPtr parser, IInputPtr input, IOutputPtr output)
: parser_(std::move(parser))
, input_(std::move(input))
, output_(std::move(output)) { }

IInterpreterPtr BytecodeInterpreterFactory::Create() const {
    auto values = std::make_shared<Environment>();
    auto machine = std::make_shared<Machine>(values, input_, output_);

    return std::make_shared<BytecodeInterpreter>(parser_, std::make_shared<Compiler>(), machine);
}
