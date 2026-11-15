#pragma once

#include "IInterpreterFactory.hpp"
#include "parser/IParser.hpp"
#include "runtime/io/IInput.hpp"
#include "runtime/io/IOutput.hpp"

class TreeInterpreterFactory final : public IInterpreterFactory {
public:
    TreeInterpreterFactory(IParserPtr parser, IInputPtr input, IOutputPtr output);

    IInterpreterPtr Create() const override;

private:
    IParserPtr parser_;
    IInputPtr input_;
    IOutputPtr output_;
};
