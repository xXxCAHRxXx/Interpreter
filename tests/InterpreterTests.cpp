#include <memory>
#include <sstream>

#include <gtest/gtest.h>

#include "TestSupport.hpp"
#include "backend/runtime/environment/Environment.hpp"
#include "backend/tree/evaluator/factory/ExpressionEvaluatorFactory.hpp"
#include "interpreter/TreeInterpreter.hpp"
#include "backend/runtime/io/StreamInput.hpp"
#include "backend/runtime/io/StreamOutput.hpp"
#include "backend/tree/statement/StatementExecutor.hpp"

TEST(InterpreterTest, RunsJsonProgramFromInputToOutput) {
    std::istringstream runtime_input("40");
    std::ostringstream runtime_output;
    auto environment = std::make_shared<Environment>();
    auto executor = std::make_shared<StatementExecutor>(environment,
                                                        std::make_shared<ExpressionEvaluatorFactory>(),
                                                        std::make_shared<StreamInput>(runtime_input),
                                                        std::make_shared<StreamOutput>(runtime_output));
    TreeInterpreter interpreter(CreateTestParser(), executor);
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
