#include <cstdlib>
#include <exception>
#include <fstream>
#include <iostream>
#include <memory>
#include <string_view>
#include <utility>

#include "binary_operator/registry/factory/BinaryOperatorRegistryFactory.hpp"
#include "error/InterpreterError.hpp"
#include "parser/IParser.hpp"
#include "parser/json/JsonParser.hpp"
#include "parser/json/expression_parser/factory/JsonExpressionParserRegistryFactory.hpp"
#include "parser/json/statement_parser/factory/JsonStatementParserRegistryFactory.hpp"
#include "runtime/interpreter/IInterpreter.hpp"
#include "runtime/interpreter/factory/BytecodeInterpreterFactory.hpp"
#include "runtime/interpreter/factory/IInterpreterFactory.hpp"
#include "runtime/interpreter/factory/TreeInterpreterFactory.hpp"
#include "runtime/io/StreamInput.hpp"
#include "runtime/io/StreamOutput.hpp"

IParserPtr createParser() {
    JsonExpressionParserRegistryFactory expressions(std::make_shared<BinaryOperatorRegistryFactory>());

    return std::make_shared<JsonParser>(JsonStatementParserRegistryFactory().Create(), expressions.Create());
}

IInterpreterFactoryPtr createInterpreterFactory(bool bytecode) {
    IParserPtr parser = createParser();
    auto input = std::make_shared<StreamInput>(std::cin);
    auto output = std::make_shared<StreamOutput>(std::cout);

    if (bytecode) {
        return std::make_shared<BytecodeInterpreterFactory>(parser, input, output);
    }

    return std::make_shared<TreeInterpreterFactory>(parser, input, output);
}

int main(int argc, char** argv) {
    const bool bytecode = argc == 3 && std::string_view(argv[1]) == "--bytecode";
    const bool interpret = argc == 2 && std::string_view(argv[1]) != "--bytecode";
    if (!bytecode && !interpret) {
        std::cerr << "usage: " << argv[0] << " [--bytecode] <program.json>\n";
        return EXIT_FAILURE;
    }

    try {
        std::ifstream file(argv[argc - 1]);
        if (!file) {
            std::cerr << "cannot open file: " << argv[argc - 1] << '\n';
            return EXIT_FAILURE;
        }

        IInterpreterPtr interpreter = createInterpreterFactory(bytecode)->Create();
        interpreter->Run(file);
    } catch (const InterpreterError& error) {
        std::cerr << "error: " << error.what() << '\n';
        return EXIT_FAILURE;
    } catch (const std::exception& error) {
        std::cerr << "internal error: " << error.what() << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
