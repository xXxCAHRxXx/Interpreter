#pragma once

#include <iosfwd>

#include "IInterpreter.hpp"
#include "parser/IParser.hpp"
#include "runtime/statement/IStatementExecutor.hpp"

class TreeInterpreter final : public IInterpreter {
public:
    TreeInterpreter(IParserPtr parser, IStatementExecutorPtr executor);

    void Run(std::istream& program) const override;

private:
    IParserPtr parser_;
    IStatementExecutorPtr executor_;
};
