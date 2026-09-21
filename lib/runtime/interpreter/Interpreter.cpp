#include "Interpreter.hpp"

#include <utility>

Interpreter::Interpreter(IParserPtr parser, IStatementExecutorPtr executor)
: parser_(std::move(parser))
, executor_(std::move(executor))
{ }

void Interpreter::Run(std::istream& program) const {
    IStmtPtr tree = parser_->Parse(program);

    executor_->Execute(tree);
}