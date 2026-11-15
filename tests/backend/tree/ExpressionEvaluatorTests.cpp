#include <memory>

#include <gtest/gtest.h>

#include "TestSupport.hpp"
#include "backend/runtime/environment/Environment.hpp"
#include "backend/tree/evaluator/ExpressionEvaluator.hpp"
#include "binary_operator/arithmetic/Add.hpp"
#include "binary_operator/arithmetic/Div.hpp"
#include "binary_operator/arithmetic/Mul.hpp"
#include "binary_operator/arithmetic/Sub.hpp"
#include "error/DivisionByZero.hpp"
#include "error/UndefinedVariable.hpp"

TEST(ExpressionEvaluatorTest, Evaluate_Const_ReturnsValue) {
    ExpressionEvaluator evaluator(std::make_shared<Environment>());

    Value value = evaluator.Evaluate(Constant(17));

    EXPECT_EQ(value, Value(17));
}

TEST(ExpressionEvaluatorTest, Evaluate_DefinedVar_ReturnsVariableValue) {
    auto environment = std::make_shared<Environment>();
    environment->Set("x", Value(9));
    ExpressionEvaluator evaluator(environment);

    Value value = evaluator.Evaluate(Variable("x"));

    EXPECT_EQ(value, Value(9));
}

TEST(ExpressionEvaluatorTest, Evaluate_UndefinedVar_ThrowsUndefinedVariable) {
    ExpressionEvaluator evaluator(std::make_shared<Environment>());

    EXPECT_THROW(evaluator.Evaluate(Variable("x")), UndefinedVariable);
}

TEST(ExpressionEvaluatorTest, Evaluate_NestedBinOp_ReturnsResult) {
    auto environment = std::make_shared<Environment>();
    environment->Set("x", Value(4));
    ExpressionEvaluator evaluator(environment);
    IExprPtr expression = Binary<Add>(Constant(2), Binary<Mul>(Variable("x"), Constant(3)));

    Value value = evaluator.Evaluate(expression);

    EXPECT_EQ(value, Value(14));
}

TEST(ExpressionEvaluatorTest, Evaluate_NonCommutativeBinOp_KeepsOperandOrder) {
    ExpressionEvaluator evaluator(std::make_shared<Environment>());

    Value value = evaluator.Evaluate(Binary<Sub>(Constant(10), Constant(3)));

    EXPECT_EQ(value, Value(7));
}

TEST(ExpressionEvaluatorTest, Evaluate_DivisionByZero_ThrowsDivisionByZero) {
    ExpressionEvaluator evaluator(std::make_shared<Environment>());
    IExprPtr expression = Binary<Div>(Constant(1), Constant(0));

    EXPECT_THROW(evaluator.Evaluate(expression), DivisionByZero);
}

TEST(ExpressionEvaluatorTest, Evaluate_SameEvaluatorTwice_ReturnsIndependentResults) {
    ExpressionEvaluator evaluator(std::make_shared<Environment>());
    evaluator.Evaluate(Binary<Add>(Constant(1), Constant(2)));

    Value value = evaluator.Evaluate(Constant(5));

    EXPECT_EQ(value, Value(5));
}
