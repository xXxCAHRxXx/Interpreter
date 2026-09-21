#include <cstdlib>
#include <exception>
#include <fstream>
#include <iostream>
#include <memory>
#include <utility>

#include "binary_operator/Add.hpp"
#include "binary_operator/And.hpp"
#include "binary_operator/BinaryOperatorRegistry.hpp"
#include "binary_operator/Div.hpp"
#include "binary_operator/Eq.hpp"
#include "binary_operator/Ge.hpp"
#include "binary_operator/Gt.hpp"
#include "binary_operator/Le.hpp"
#include "binary_operator/Lt.hpp"
#include "binary_operator/Mod.hpp"
#include "binary_operator/Mul.hpp"
#include "binary_operator/Ne.hpp"
#include "binary_operator/Or.hpp"
#include "binary_operator/Sub.hpp"
#include "error/InterpreterError.hpp"
#include "parser/json/JsonParser.hpp"
#include "parser/json/expression_parser/BinOpParser.hpp"
#include "parser/json/expression_parser/ConstParser.hpp"
#include "parser/json/expression_parser/VarParser.hpp"
#include "parser/json/statement_parser/AssignParser.hpp"
#include "parser/json/statement_parser/DoWhileParser.hpp"
#include "parser/json/statement_parser/IfParser.hpp"
#include "parser/json/statement_parser/ReadParser.hpp"
#include "parser/json/statement_parser/SeqParser.hpp"
#include "parser/json/statement_parser/SkipParser.hpp"
#include "parser/json/statement_parser/WhileParser.hpp"
#include "parser/json/statement_parser/WriteParser.hpp"
#include "runtime/environment/Environment.hpp"
#include "runtime/evaluator/factory/ExpressionEvaluatorFactory.hpp"
#include "runtime/interpreter/Interpreter.hpp"
#include "runtime/io/StreamInput.hpp"
#include "runtime/io/StreamOutput.hpp"
#include "runtime/statement/StatementExecutor.hpp"

BinaryOperatorRegistryPtr createOperators() {
    BinaryOperatorRegistry::Builder builder;

    builder.Add("+",  std::make_shared<Add>())
           .Add("-",  std::make_shared<Sub>())
           .Add("*",  std::make_shared<Mul>())
           .Add("/",  std::make_shared<Div>())
           .Add("%",  std::make_shared<Mod>())
           .Add("<",  std::make_shared<Lt>())
           .Add("<=", std::make_shared<Le>())
           .Add(">",  std::make_shared<Gt>())
           .Add(">=", std::make_shared<Ge>())
           .Add("==", std::make_shared<Eq>())
           .Add("!=", std::make_shared<Ne>())
           .Add("&&", std::make_shared<And>())
           .Add("!!", std::make_shared<Or>());

    return std::make_shared<BinaryOperatorRegistry>(builder.Build());
}

JsonStatementParserRegistry createStatementParsers() {
    JsonStatementParserRegistry::Builder builder;

    builder.Add("skip",  std::make_shared<SkipParser>())
           .Add("assn",  std::make_shared<AssignParser>())
           .Add("read",  std::make_shared<ReadParser>())
           .Add("write", std::make_shared<WriteParser>())
           .Add("if",    std::make_shared<IfParser>())
           .Add("while", std::make_shared<WhileParser>())
           .Add("do",    std::make_shared<DoWhileParser>())
           .Add("seq",   std::make_shared<SeqParser>());

    return builder.Build();
}

JsonExpressionParserRegistry createExpressionParsers(BinaryOperatorRegistryPtr operators) {
    JsonExpressionParserRegistry::Builder builder;

    builder.Add("var",   std::make_shared<VarParser>())
           .Add("const", std::make_shared<ConstParser>())
           .Add("binop", std::make_shared<BinOpParser>(std::move(operators)));

    return builder.Build();
}

Interpreter createInterpreter() {
    auto parser = std::make_shared<JsonParser>(createStatementParsers(),
                                               createExpressionParsers(createOperators()));

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