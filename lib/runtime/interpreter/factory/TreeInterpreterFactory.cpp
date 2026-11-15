#include "TreeInterpreterFactory.hpp"

#include <memory>
#include <utility>

#include "runtime/environment/Environment.hpp"
#include "runtime/evaluator/factory/ExpressionEvaluatorFactory.hpp"
#include "runtime/interpreter/TreeInterpreter.hpp"
#include "runtime/statement/StatementExecutor.hpp"

TreeInterpreterFactory::TreeInterpreterFactory(IParserPtr parser, IInputPtr input, IOutputPtr output)
: parser_(std::move(parser))
, input_(std::move(input))
, output_(std::move(output)) { }

IInterpreterPtr TreeInterpreterFactory::Create() const {
    auto values = std::make_shared<Environment>();
    auto executor =
        std::make_shared<StatementExecutor>(values, std::make_shared<ExpressionEvaluatorFactory>(), input_, output_);

    return std::make_shared<TreeInterpreter>(parser_, executor);
}
