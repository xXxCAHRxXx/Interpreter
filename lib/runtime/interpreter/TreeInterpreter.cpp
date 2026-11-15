#include "TreeInterpreter.hpp"

#include <utility>

TreeInterpreter::TreeInterpreter(IParserPtr parser, IStatementExecutorPtr executor)
: parser_(std::move(parser))
, executor_(std::move(executor))
{ }

void TreeInterpreter::Run(std::istream& program) const {
    IStmtPtr tree = parser_->Parse(program);

    executor_->Execute(tree);
}
