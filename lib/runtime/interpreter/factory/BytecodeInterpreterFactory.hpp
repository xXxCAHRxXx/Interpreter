#pragma once

#include "IInterpreterFactory.hpp"
#include "parser/IParser.hpp"
#include "runtime/io/IInput.hpp"
#include "runtime/io/IOutput.hpp"

class BytecodeInterpreterFactory final : public IInterpreterFactory {
public:
    BytecodeInterpreterFactory(IParserPtr parser, IInputPtr input, IOutputPtr output);

    IInterpreterPtr Create() const override;

private:
    IParserPtr parser_;
    IInputPtr input_;
    IOutputPtr output_;
};
