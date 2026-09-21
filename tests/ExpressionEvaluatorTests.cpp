#include <memory>

#include <gtest/gtest.h>

#include "ast/expression/BinOp.hpp"
#include "ast/expression/Const.hpp"
#include "ast/expression/Var.hpp"
#include "binary_operator/Add.hpp"
#include "binary_operator/Mul.hpp"
#include "runtime/environment/Environment.hpp"
#include "runtime/evaluator/ExpressionEvaluator.hpp"

TEST(ExpressionEvaluatorTest, EvaluatesConstants) {
    auto environment = std::make_shared<Environment>();
    ExpressionEvaluator evaluator(environment);

    EXPECT_EQ(evaluator.Evaluate(std::make_shared<Const>(Value(17))), Value(17));
}

TEST(ExpressionEvaluatorTest, ReadsVariables) {
    auto environment = std::make_shared<Environment>();
    environment->Set("x", Value(9));
    ExpressionEvaluator evaluator(environment);

    EXPECT_EQ(evaluator.Evaluate(std::make_shared<Var>("x")), Value(9));
}

TEST(ExpressionEvaluatorTest, EvaluatesNestedBinaryExpressions) {
    auto environment = std::make_shared<Environment>();
    environment->Set("x", Value(4));
    ExpressionEvaluator evaluator(environment);

    auto multiplication = std::make_shared<BinOp>(
        std::make_shared<Mul>(),
        std::make_shared<Var>("x"),
        std::make_shared<Const>(Value(3))
    );
    auto expression = std::make_shared<BinOp>(
        std::make_shared<Add>(),
        std::make_shared<Const>(Value(2)),
        multiplication
    );

    EXPECT_EQ(evaluator.Evaluate(expression), Value(14));
}
