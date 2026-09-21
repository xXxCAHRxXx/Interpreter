#include <memory>
#include <sstream>

#include <gtest/gtest.h>

#include "TestSupport.hpp"
#include "runtime/environment/Environment.hpp"
#include "runtime/evaluator/factory/ExpressionEvaluatorFactory.hpp"
#include "runtime/interpreter/Interpreter.hpp"
#include "runtime/io/StreamInput.hpp"
#include "runtime/io/StreamOutput.hpp"
#include "runtime/statement/StatementExecutor.hpp"

TEST(InterpreterTest, RunsJsonProgramFromInputToOutput) {
    std::istringstream runtime_input("40");
    std::ostringstream runtime_output;
    auto environment = std::make_shared<Environment>();
    auto executor = std::make_shared<StatementExecutor>(
        environment,
        std::make_shared<ExpressionEvaluatorFactory>(),
        std::make_shared<StreamInput>(runtime_input),
        std::make_shared<StreamOutput>(runtime_output)
    );
    Interpreter interpreter(CreateTestParser(), executor);
    std::istringstream program(R"(
        {
          "seq": {
            "left": { "read": "n" },
            "right": {
              "seq": {
                "left": {
                  "assn": {
                    "dst": "answer",
                    "src": {
                      "binop": "+",
                      "left": { "var": "n" },
                      "right": { "const": 2 }
                    }
                  }
                },
                "right": { "write": { "var": "answer" } }
              }
            }
          }
        }
    )");

    interpreter.Run(program);

    EXPECT_EQ(runtime_output.str(), "42\n");
}
