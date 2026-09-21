#pragma once

#include <iosfwd>

#include "parser/IParser.hpp"
#include "runtime/statement/IStatementExecutor.hpp"

class Interpreter {
public:
    Interpreter(IParserPtr parser, IStatementExecutorPtr executor);

    void Run(std::istream& program) const;

private:
    IParserPtr parser_;
    IStatementExecutorPtr executor_;
};