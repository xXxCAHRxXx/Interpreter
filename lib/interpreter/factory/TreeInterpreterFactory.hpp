#pragma once

#include "IInterpreterFactory.hpp"
#include "frontend/parser/IParser.hpp"
#include "backend/runtime/io/IInput.hpp"
#include "backend/runtime/io/IOutput.hpp"

class TreeInterpreterFactory final : public IInterpreterFactory {
public:
    TreeInterpreterFactory(IParserPtr parser, IInputPtr input, IOutputPtr output);

    IInterpreterPtr Create() const override;

private:
    IParserPtr parser_;
    IInputPtr input_;
    IOutputPtr output_;
};
