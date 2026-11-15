#include "BytecodeInterpreter.hpp"

#include <utility>

BytecodeInterpreter::BytecodeInterpreter(IParserPtr parser, ICompilerPtr compiler, IMachinePtr machine)
: parser_(std::move(parser))
, compiler_(std::move(compiler))
, machine_(std::move(machine)) { }

void BytecodeInterpreter::Run(std::istream& program) const {
    IStmtPtr tree = parser_->Parse(program);
    ProgramPtr bytecode = compiler_->Compile(tree);

    machine_->Execute(bytecode);
}
