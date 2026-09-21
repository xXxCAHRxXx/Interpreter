#include <memory>
#include <vector>

#include <gtest/gtest.h>

#include "TestSupport.hpp"
#include "ast/expression/BinOp.hpp"
#include "ast/expression/Const.hpp"
#include "ast/expression/Var.hpp"
#include "ast/statement/Assign.hpp"
#include "ast/statement/Block.hpp"
#include "ast/statement/DoWhile.hpp"
#include "ast/statement/If.hpp"
#include "ast/statement/Read.hpp"
#include "ast/statement/Skip.hpp"
#include "ast/statement/While.hpp"
#include "ast/statement/Write.hpp"
#include "binary_operator/Add.hpp"
#include "binary_operator/Lt.hpp"
#include "runtime/environment/Environment.hpp"
#include "runtime/evaluator/factory/ExpressionEvaluatorFactory.hpp"
#include "runtime/statement/StatementExecutor.hpp"

namespace {

struct ExecutorFixture {
    ExecutorFixture(std::vector<Value> input_values = {})
    : environment(std::make_shared<Environment>())
    , input(std::make_shared<StubInput>(std::move(input_values)))
    , output(std::make_shared<RecordingOutput>())
    , executor(environment,
               std::make_shared<ExpressionEvaluatorFactory>(),
               input,
               output)
    { }

    std::shared_ptr<Environment> environment;
    std::shared_ptr<StubInput> input;
    std::shared_ptr<RecordingOutput> output;
    StatementExecutor executor;
};

IExprPtr Constant(int value) {
    return std::make_shared<Const>(Value(value));
}

IExprPtr Variable(const char* name) {
    return std::make_shared<Var>(name);
}

IExprPtr AddExpression(IExprPtr lhs, IExprPtr rhs) {
    return std::make_shared<BinOp>(std::make_shared<Add>(), std::move(lhs), std::move(rhs));
}

} // namespace

TEST(StatementExecutorTest, AssignsAndWritesValues) {
    ExecutorFixture fixture;
    std::vector<IStmtPtr> statements;
    statements.push_back(std::make_shared<Assign>("x", AddExpression(Constant(2), Constant(3))));
    statements.push_back(std::make_shared<Write>(Variable("x")));

    fixture.executor.Execute(std::make_shared<Block>(std::move(statements)));

    EXPECT_EQ(fixture.environment->Get("x"), Value(5));
    ASSERT_EQ(fixture.output->values.size(), 1U);
    EXPECT_EQ(fixture.output->values[0], Value(5));
}

TEST(StatementExecutorTest, ReadsValueIntoEnvironment) {
    ExecutorFixture fixture({Value(42)});

    fixture.executor.Execute(std::make_shared<Read>("answer"));

    EXPECT_EQ(fixture.environment->Get("answer"), Value(42));
}

TEST(StatementExecutorTest, ExecutesSelectedIfBranch) {
    ExecutorFixture fixture;
    auto statement = std::make_shared<If>(
        Constant(1),
        std::make_shared<Write>(Constant(10)),
        std::make_shared<Write>(Constant(20))
    );

    fixture.executor.Execute(statement);

    ASSERT_EQ(fixture.output->values.size(), 1U);
    EXPECT_EQ(fixture.output->values[0], Value(10));
}

TEST(StatementExecutorTest, ExecutesWhileLoopInOrder) {
    ExecutorFixture fixture;
    fixture.environment->Set("x", Value(0));

    std::vector<IStmtPtr> body_statements;
    body_statements.push_back(std::make_shared<Write>(Variable("x")));
    body_statements.push_back(std::make_shared<Assign>(
        "x",
        AddExpression(Variable("x"), Constant(1))
    ));

    auto condition = std::make_shared<BinOp>(
        std::make_shared<Lt>(),
        Variable("x"),
        Constant(3)
    );
    auto loop = std::make_shared<While>(
        condition,
        std::make_shared<Block>(std::move(body_statements))
    );

    fixture.executor.Execute(loop);

    EXPECT_EQ(fixture.environment->Get("x"), Value(3));
    EXPECT_EQ(fixture.output->values, std::vector<Value>({Value(0), Value(1), Value(2)}));
}

TEST(StatementExecutorTest, ExecutesDoWhileBodyAtLeastOnce) {
    ExecutorFixture fixture;
    auto loop = std::make_shared<DoWhile>(
        std::make_shared<Write>(Constant(7)),
        Constant(0)
    );

    fixture.executor.Execute(loop);

    EXPECT_EQ(fixture.output->values, std::vector<Value>({Value(7)}));
}

TEST(StatementExecutorTest, SkipHasNoEffect) {
    ExecutorFixture fixture;

    fixture.executor.Execute(std::make_shared<Skip>());

    EXPECT_TRUE(fixture.output->values.empty());
}
