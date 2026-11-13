#include <cstdlib>
#include <exception>
#include <fstream>
#include <iostream>
#include <memory>
#include <utility>

#include "binary_operator/registry/factory/BinaryOperatorRegistryFactory.hpp"
#include "error/InterpreterError.hpp"
#include "parser/json/JsonParser.hpp"
#include "parser/json/expression_parser/factory/JsonExpressionParserRegistryFactory.hpp"
#include "parser/json/statement_parser/factory/JsonStatementParserRegistryFactory.hpp"
#include "runtime/environment/Environment.hpp"
#include "runtime/evaluator/factory/ExpressionEvaluatorFactory.hpp"
#include "runtime/interpreter/Interpreter.hpp"
#include "runtime/io/StreamInput.hpp"
#include "runtime/io/StreamOutput.hpp"
#include "runtime/statement/StatementExecutor.hpp"

Interpreter createInterpreter() {
    JsonExpressionParserRegistryFactory expressions(std::make_shared<BinaryOperatorRegistryFactory>());

    auto parser = std::make_shared<JsonParser>(JsonStatementParserRegistryFactory().Create(), expressions.Create());

    auto executor = std::make_shared<StatementExecutor>(std::make_shared<Environment>(),
                                                        std::make_shared<ExpressionEvaluatorFactory>(),
                                                        std::make_shared<StreamInput>(std::cin),
                                                        std::make_shared<StreamOutput>(std::cout));

    return Interpreter(std::move(parser), std::move(executor));
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "usage: " << argv[0] << " <program.json>\n";
        return EXIT_FAILURE;
    }

    try {
        std::ifstream file(argv[1]);
        if (!file) {
            std::cerr << "cannot open file: " << argv[1] << '\n';
            return EXIT_FAILURE;
        }

        Interpreter interpreter = createInterpreter();
        interpreter.Run(file);
    } catch (const InterpreterError& error) {
        std::cerr << "error: " << error.what() << '\n';
        return EXIT_FAILURE;
    } catch (const std::exception& error) {
        std::cerr << "internal error: " << error.what() << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
