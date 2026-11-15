#pragma once

#include <iosfwd>

#include "IInterpreter.hpp"
#include "backend/bytecode/compiler/ICompiler.hpp"
#include "frontend/parser/IParser.hpp"
#include "backend/bytecode/machine/IMachine.hpp"

class BytecodeInterpreter final : public IInterpreter {
public:
    BytecodeInterpreter(IParserPtr parser, ICompilerPtr compiler, IMachinePtr machine);

    void Run(std::istream& program) const override;

private:
    IParserPtr parser_;
    ICompilerPtr compiler_;
    IMachinePtr machine_;
};
