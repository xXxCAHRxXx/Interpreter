#include <memory>
#include <sstream>

#include <gtest/gtest.h>

#include "TestSupport.hpp"
#include "backend/runtime/environment/Environment.hpp"
#include "backend/runtime/io/StreamInput.hpp"
#include "backend/runtime/io/StreamOutput.hpp"
#include "backend/tree/evaluator/factory/ExpressionEvaluatorFactory.hpp"
#include "backend/tree/statement/StatementExecutor.hpp"
#include "binary_operator/arithmetic/Add.hpp"
#include "binary_operator/arithmetic/Sub.hpp"
#include "binary_operator/comparison/Lt.hpp"
#include "error/InvalidInput.hpp"
#include "error/UndefinedVariable.hpp"
#include "frontend/ast/statement/Assign.hpp"
#include "frontend/ast/statement/DoWhile.hpp"
#include "frontend/ast/statement/If.hpp"
#include "frontend/ast/statement/Read.hpp"
#include "frontend/ast/statement/Skip.hpp"
#include "frontend/ast/statement/While.hpp"
#include "frontend/ast/statement/Write.hpp"

class StatementExecutorTest : public ::testing::Test {
protected:
    StatementExecutorTest()
    : environment(std::make_shared<Environment>())
    , executor(environment,
               std::make_shared<ExpressionEvaluatorFactory>(),
               std::make_shared<StreamInput>(input),
               std::make_shared<StreamOutput>(output))
    { }

    std::istringstream input;
    std::ostringstream output;
    std::shared_ptr<Environment> environment;
    StatementExecutor executor;
};

TEST_F(StatementExecutorTest, Execute_AssignConstant_StoresVariable) {
    auto statement = std::make_shared<Assign>("x", Constant(5));

    executor.Execute(statement);

    EXPECT_EQ(environment->Get("x"), Value(5));
}

TEST_F(StatementExecutorTest, Execute_AssignUsingSameVariable_UpdatesVariable) {
    environment->Set("x", Value(5));
    auto statement = std::make_shared<Assign>("x", Binary<Add>(Variable("x"), Constant(1)));

    executor.Execute(statement);

    EXPECT_EQ(environment->Get("x"), Value(6));
}

TEST_F(StatementExecutorTest, Execute_AssignUndefinedVariable_ThrowsUndefinedVariable) {
    auto statement = std::make_shared<Assign>("x", Variable("y"));

    EXPECT_THROW(executor.Execute(statement), UndefinedVariable);
}

TEST_F(StatementExecutorTest, Execute_Read_StoresInputValue) {
    input.str("42");
    auto statement = std::make_shared<Read>("x");

    executor.Execute(statement);

    EXPECT_EQ(environment->Get("x"), Value(42));
}

TEST_F(StatementExecutorTest, Execute_ReadWithoutInput_ThrowsInvalidInput) {
    auto statement = std::make_shared<Read>("x");

    EXPECT_THROW(executor.Execute(statement), InvalidInput);
}

TEST_F(StatementExecutorTest, Execute_Write_WritesExpressionValue) {
    auto statement = std::make_shared<Write>(Binary<Add>(Constant(2), Constant(3)));

    executor.Execute(statement);

    EXPECT_EQ(output.str(), "5\n");
}

TEST_F(StatementExecutorTest, Execute_WriteUndefinedVariable_ThrowsUndefinedVariable) {
    auto statement = std::make_shared<Write>(Variable("x"));

    EXPECT_THROW(executor.Execute(statement), UndefinedVariable);
}

TEST_F(StatementExecutorTest, Execute_IfWithTrueCondition_ExecutesThenBranch) {
    auto statement = std::make_shared<If>(Constant(1),
                                          std::make_shared<Write>(Constant(1)),
                                          std::make_shared<Write>(Constant(2)));

    executor.Execute(statement);

    EXPECT_EQ(output.str(), "1\n");
}

TEST_F(StatementExecutorTest, Execute_IfWithFalseCondition_ExecutesElseBranch) {
    auto statement = std::make_shared<If>(Constant(0),
                                          std::make_shared<Write>(Constant(1)),
                                          std::make_shared<Write>(Constant(2)));

    executor.Execute(statement);

    EXPECT_EQ(output.str(), "2\n");
}

TEST_F(StatementExecutorTest, Execute_IfWithNegativeCondition_ExecutesThenBranch) {
    auto statement = std::make_shared<If>(Constant(-1),
                                          std::make_shared<Write>(Constant(1)),
                                          std::make_shared<Write>(Constant(2)));

    executor.Execute(statement);

    EXPECT_EQ(output.str(), "1\n");
}

TEST_F(StatementExecutorTest, Execute_IfWithUndefinedVariableInUntakenBranch_DoesNotThrow) {
    auto statement = std::make_shared<If>(Constant(0),
                                          std::make_shared<Write>(Variable("missing")),
                                          std::make_shared<Write>(Constant(2)));

    executor.Execute(statement);

    EXPECT_EQ(output.str(), "2\n");
}

TEST_F(StatementExecutorTest, Execute_WhileWithFalseCondition_SkipsBody) {
    auto statement = std::make_shared<While>(Constant(0), std::make_shared<Write>(Constant(1)));

    executor.Execute(statement);

    EXPECT_EQ(output.str(), "");
}

TEST_F(StatementExecutorTest, Execute_WhileCountingLoop_RepeatsBodyWhileConditionHolds) {
    environment->Set("i", Value(0));
    auto statement = std::make_shared<While>(
        Binary<Lt>(Variable("i"), Constant(3)),
        Sequence({
            std::make_shared<Write>(Variable("i")),
            std::make_shared<Assign>("i", Binary<Add>(Variable("i"), Constant(1)))
        })
    );

    executor.Execute(statement);

    EXPECT_EQ(output.str(), "0\n1\n2\n");
}

TEST_F(StatementExecutorTest, Execute_WhileWithUndefinedCondition_ThrowsUndefinedVariable) {
    auto statement = std::make_shared<While>(Variable("missing"), std::make_shared<Skip>());

    EXPECT_THROW(executor.Execute(statement), UndefinedVariable);
}

TEST_F(StatementExecutorTest, Execute_DoWhileWithFalseCondition_ExecutesBodyOnce) {
    auto statement = std::make_shared<DoWhile>(std::make_shared<Write>(Constant(7)), Constant(0));

    executor.Execute(statement);

    EXPECT_EQ(output.str(), "7\n");
}

TEST_F(StatementExecutorTest, Execute_DoWhileCountdown_RepeatsBodyWhileConditionHolds) {
    environment->Set("n", Value(3));
    auto statement = std::make_shared<DoWhile>(
        Sequence({
            std::make_shared<Write>(Variable("n")),
            std::make_shared<Assign>("n", Binary<Sub>(Variable("n"), Constant(1)))
        }),
        Variable("n")
    );

    executor.Execute(statement);

    EXPECT_EQ(output.str(), "3\n2\n1\n");
}

TEST_F(StatementExecutorTest, Execute_Skip_ChangesNothing) {
    auto statement = std::make_shared<Skip>();

    executor.Execute(statement);

    EXPECT_EQ(output.str(), "");
    EXPECT_THROW(environment->Get("x"), UndefinedVariable);
}

TEST_F(StatementExecutorTest, Execute_EmptyBlock_ChangesNothing) {
    IStmtPtr statement = Sequence({});

    executor.Execute(statement);

    EXPECT_EQ(output.str(), "");
}

TEST_F(StatementExecutorTest, Execute_Block_ExecutesStatementsInOrder) {
    IStmtPtr statement = Sequence({
        std::make_shared<Assign>("x", Constant(1)),
        std::make_shared<Write>(Variable("x")),
        std::make_shared<Assign>("x", Constant(2)),
        std::make_shared<Write>(Variable("x"))
    });

    executor.Execute(statement);

    EXPECT_EQ(output.str(), "1\n2\n");
}

TEST_F(StatementExecutorTest, Execute_BlockWithFailingStatement_KeepsOutputBeforeError) {
    IStmtPtr statement = Sequence({
        std::make_shared<Write>(Constant(1)),
        std::make_shared<Write>(Variable("missing")),
        std::make_shared<Write>(Constant(2))
    });

    EXPECT_THROW(executor.Execute(statement), UndefinedVariable);
    EXPECT_EQ(output.str(), "1\n");
}
