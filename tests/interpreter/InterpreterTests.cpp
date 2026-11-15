#include <cstdint>
#include <memory>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include <gtest/gtest.h>
#include <nlohmann/json.hpp>

#include "TestSupport.hpp"
#include "backend/runtime/io/StreamInput.hpp"
#include "backend/runtime/io/StreamOutput.hpp"
#include "error/DivisionByZero.hpp"
#include "error/InvalidInput.hpp"
#include "error/SyntaxError.hpp"
#include "error/UndefinedVariable.hpp"
#include "interpreter/factory/BytecodeInterpreterFactory.hpp"
#include "interpreter/factory/TreeInterpreterFactory.hpp"

namespace node {

nlohmann::json Const(std::int32_t value) {
    return {{"const", value}};
}

nlohmann::json Var(const std::string& name) {
    return {{"var", name}};
}

nlohmann::json BinOp(const std::string& op, nlohmann::json lhs, nlohmann::json rhs) {
    return {{"binop", op}, {"left", std::move(lhs)}, {"right", std::move(rhs)}};
}

nlohmann::json Skip() {
    return "skip";
}

nlohmann::json Assign(const std::string& name, nlohmann::json value) {
    return {{"assn", {{"dst", name}, {"src", std::move(value)}}}};
}

nlohmann::json Read(const std::string& name) {
    return {{"read", name}};
}

nlohmann::json Write(nlohmann::json value) {
    return {{"write", std::move(value)}};
}

nlohmann::json If(nlohmann::json cond, nlohmann::json then_branch, nlohmann::json else_branch) {
    return {{"if", {{"cond", std::move(cond)}, {"then", std::move(then_branch)}, {"else", std::move(else_branch)}}}};
}

nlohmann::json While(nlohmann::json cond, nlohmann::json body) {
    return {{"while", {{"cond", std::move(cond)}, {"body", std::move(body)}}}};
}

nlohmann::json DoWhile(nlohmann::json body, nlohmann::json cond) {
    return {{"do", {{"body", std::move(body)}, {"cond", std::move(cond)}}}};
}

nlohmann::json Seq(std::vector<nlohmann::json> statements) {
    nlohmann::json result = std::move(statements.back());
    statements.pop_back();

    while (!statements.empty()) {
        result = {{"seq", {{"left", std::move(statements.back())}, {"right", std::move(result)}}}};
        statements.pop_back();
    }

    return result;
}

}

using InterpreterFactoryCreator = IInterpreterFactoryPtr (*)(IParserPtr, IInputPtr, IOutputPtr);

IInterpreterFactoryPtr CreateTreeInterpreterFactory(IParserPtr parser, IInputPtr input, IOutputPtr output) {
    return std::make_shared<TreeInterpreterFactory>(std::move(parser), std::move(input), std::move(output));
}

IInterpreterFactoryPtr CreateBytecodeInterpreterFactory(IParserPtr parser, IInputPtr input, IOutputPtr output) {
    return std::make_shared<BytecodeInterpreterFactory>(std::move(parser), std::move(input), std::move(output));
}

class InterpreterTest : public ::testing::TestWithParam<InterpreterFactoryCreator> {
protected:
    void Run(const std::string& program) {
        std::istringstream source(program);
        IInterpreterFactoryPtr factory = GetParam()(CreateTestParser(),
                                                    std::make_shared<StreamInput>(input),
                                                    std::make_shared<StreamOutput>(output));

        factory->Create()->Run(source);
    }

    void Run(const nlohmann::json& program) {
        Run(program.dump());
    }

    std::istringstream input;
    std::ostringstream output;
};

TEST_P(InterpreterTest, Run_WriteConstant_WritesConstant) {
    nlohmann::json program = node::Write(node::Const(42));

    Run(program);

    EXPECT_EQ(output.str(), "42\n");
}

TEST_P(InterpreterTest, Run_ReadThenWrite_EchoesInput) {
    input.str("17");
    nlohmann::json program = node::Seq({node::Read("x"), node::Write(node::Var("x"))});

    Run(program);

    EXPECT_EQ(output.str(), "17\n");
}

TEST_P(InterpreterTest, Run_SeveralReads_ReadsInputInOrder) {
    input.str("10 3");
    nlohmann::json program = node::Seq({
        node::Read("a"),
        node::Read("b"),
        node::Write(node::BinOp("-", node::Var("a"), node::Var("b")))
    });

    Run(program);

    EXPECT_EQ(output.str(), "7\n");
}

TEST_P(InterpreterTest, Run_ArithmeticExpression_WritesResult) {
    nlohmann::json program = node::Write(node::BinOp(
        "-",
        node::BinOp("*", node::BinOp("+", node::Const(2), node::Const(3)), node::Const(4)),
        node::BinOp("/", node::Const(6), node::Const(2))
    ));

    Run(program);

    EXPECT_EQ(output.str(), "17\n");
}

TEST_P(InterpreterTest, Run_Comparisons_WriteOneOrZero) {
    nlohmann::json program = node::Seq({
        node::Write(node::BinOp("<", node::Const(1), node::Const(2))),
        node::Write(node::BinOp("<=", node::Const(2), node::Const(2))),
        node::Write(node::BinOp(">", node::Const(3), node::Const(4))),
        node::Write(node::BinOp(">=", node::Const(4), node::Const(5))),
        node::Write(node::BinOp("==", node::Const(5), node::Const(5))),
        node::Write(node::BinOp("!=", node::Const(5), node::Const(5)))
    });

    Run(program);

    EXPECT_EQ(output.str(), "1\n1\n0\n0\n1\n0\n");
}

TEST_P(InterpreterTest, Run_LogicalOperators_WriteOneOrZero) {
    nlohmann::json program = node::Seq({
        node::Write(node::BinOp("&&", node::Const(1), node::Const(0))),
        node::Write(node::BinOp("&&", node::Const(2), node::Const(3))),
        node::Write(node::BinOp("!!", node::Const(0), node::Const(0))),
        node::Write(node::BinOp("!!", node::Const(0), node::Const(5)))
    });

    Run(program);

    EXPECT_EQ(output.str(), "0\n1\n0\n1\n");
}

TEST_P(InterpreterTest, Run_Reassignment_WritesLatestValue) {
    nlohmann::json program = node::Seq({
        node::Assign("x", node::Const(1)),
        node::Assign("x", node::BinOp("+", node::Var("x"), node::Const(1))),
        node::Write(node::Var("x"))
    });

    Run(program);

    EXPECT_EQ(output.str(), "2\n");
}

TEST_P(InterpreterTest, Run_IfWithTrueCondition_ExecutesThenBranch) {
    nlohmann::json program = node::If(node::Const(1), node::Write(node::Const(1)), node::Write(node::Const(2)));

    Run(program);

    EXPECT_EQ(output.str(), "1\n");
}

TEST_P(InterpreterTest, Run_IfWithFalseCondition_ExecutesElseBranch) {
    nlohmann::json program = node::If(node::Const(0), node::Write(node::Const(1)), node::Write(node::Const(2)));

    Run(program);

    EXPECT_EQ(output.str(), "2\n");
}

TEST_P(InterpreterTest, Run_ElifChain_ExecutesMatchingBranch) {
    input.str("7");
    nlohmann::json program = node::Seq({
        node::Read("x"),
        node::If(node::BinOp("<", node::Var("x"), node::Const(3)),
                 node::Write(node::Const(1)),
                 node::If(node::BinOp("==", node::Var("x"), node::Const(7)),
                          node::Write(node::Const(2)),
                          node::Write(node::Const(3))))
    });

    Run(program);

    EXPECT_EQ(output.str(), "2\n");
}

TEST_P(InterpreterTest, Run_FactorialProgram_WritesFactorial) {
    input.str("5");
    nlohmann::json program = node::Seq({
        node::Read("n"),
        node::Assign("f", node::Const(1)),
        node::While(node::BinOp(">", node::Var("n"), node::Const(0)),
                    node::Seq({
                        node::Assign("f", node::BinOp("*", node::Var("f"), node::Var("n"))),
                        node::Assign("n", node::BinOp("-", node::Var("n"), node::Const(1)))
                    })),
        node::Write(node::Var("f"))
    });

    Run(program);

    EXPECT_EQ(output.str(), "120\n");
}

TEST_P(InterpreterTest, Run_GcdProgram_WritesGreatestCommonDivisor) {
    input.str("48 18");
    nlohmann::json program = node::Seq({
        node::Read("a"),
        node::Read("b"),
        node::While(node::BinOp("!=", node::Var("b"), node::Const(0)),
                    node::Seq({
                        node::Assign("t", node::BinOp("%", node::Var("a"), node::Var("b"))),
                        node::Assign("a", node::Var("b")),
                        node::Assign("b", node::Var("t"))
                    })),
        node::Write(node::Var("a"))
    });

    Run(program);

    EXPECT_EQ(output.str(), "6\n");
}

TEST_P(InterpreterTest, Run_FibonacciProgram_WritesSequence) {
    input.str("10");
    nlohmann::json program = node::Seq({
        node::Read("n"),
        node::Assign("a", node::Const(0)),
        node::Assign("b", node::Const(1)),
        node::Assign("i", node::Const(0)),
        node::While(node::BinOp("<", node::Var("i"), node::Var("n")),
                    node::Seq({
                        node::Write(node::Var("a")),
                        node::Assign("t", node::BinOp("+", node::Var("a"), node::Var("b"))),
                        node::Assign("a", node::Var("b")),
                        node::Assign("b", node::Var("t")),
                        node::Assign("i", node::BinOp("+", node::Var("i"), node::Const(1)))
                    }))
    });

    Run(program);

    EXPECT_EQ(output.str(), "0\n1\n1\n2\n3\n5\n8\n13\n21\n34\n");
}

TEST_P(InterpreterTest, Run_SumOfDigitsProgram_WritesSum) {
    input.str("12345");
    nlohmann::json program = node::Seq({
        node::Read("n"),
        node::Assign("s", node::Const(0)),
        node::While(node::Var("n"),
                    node::Seq({
                        node::Assign("s", node::BinOp("+", node::Var("s"), node::BinOp("%", node::Var("n"), node::Const(10)))),
                        node::Assign("n", node::BinOp("/", node::Var("n"), node::Const(10)))
                    })),
        node::Write(node::Var("s"))
    });

    Run(program);

    EXPECT_EQ(output.str(), "15\n");
}

TEST_P(InterpreterTest, Run_DoWhileCountdown_WritesEveryIteration) {
    input.str("3");
    nlohmann::json program = node::Seq({
        node::Read("n"),
        node::DoWhile(node::Seq({
                          node::Write(node::Var("n")),
                          node::Assign("n", node::BinOp("-", node::Var("n"), node::Const(1)))
                      }),
                      node::BinOp(">", node::Var("n"), node::Const(0)))
    });

    Run(program);

    EXPECT_EQ(output.str(), "3\n2\n1\n");
}

TEST_P(InterpreterTest, Run_NestedLoops_WritesMultiplicationTable) {
    nlohmann::json program = node::Seq({
        node::Assign("i", node::Const(1)),
        node::While(node::BinOp("<=", node::Var("i"), node::Const(3)),
                    node::Seq({
                        node::Assign("j", node::Const(1)),
                        node::While(node::BinOp("<=", node::Var("j"), node::Const(3)),
                                    node::Seq({
                                        node::Write(node::BinOp("*", node::Var("i"), node::Var("j"))),
                                        node::Assign("j", node::BinOp("+", node::Var("j"), node::Const(1)))
                                    })),
                        node::Assign("i", node::BinOp("+", node::Var("i"), node::Const(1)))
                    }))
    });

    Run(program);

    EXPECT_EQ(output.str(), "1\n2\n3\n2\n4\n6\n3\n6\n9\n");
}

TEST_P(InterpreterTest, Run_LoopInsideIf_ExecutesLoopOnlyInTakenBranch) {
    input.str("1");
    nlohmann::json program = node::Seq({
        node::Read("flag"),
        node::Assign("i", node::Const(0)),
        node::If(node::Var("flag"),
                 node::While(node::BinOp("<", node::Var("i"), node::Const(2)),
                             node::Seq({
                                 node::Write(node::Var("i")),
                                 node::Assign("i", node::BinOp("+", node::Var("i"), node::Const(1)))
                             })),
                 node::Write(node::Const(-1)))
    });

    Run(program);

    EXPECT_EQ(output.str(), "0\n1\n");
}

TEST_P(InterpreterTest, Run_NegativeInput_HandlesNegativeNumbers) {
    input.str("-5");
    nlohmann::json program = node::Seq({
        node::Read("x"),
        node::Write(node::BinOp("*", node::Var("x"), node::Var("x"))),
        node::Write(node::BinOp("-", node::Const(0), node::Var("x")))
    });

    Run(program);

    EXPECT_EQ(output.str(), "25\n5\n");
}

TEST_P(InterpreterTest, Run_NegativeDivisionAndModulo_TruncateTowardZero) {
    nlohmann::json program = node::Seq({
        node::Write(node::BinOp("/", node::Const(-7), node::Const(2))),
        node::Write(node::BinOp("%", node::Const(-7), node::Const(2)))
    });

    Run(program);

    EXPECT_EQ(output.str(), "-3\n-1\n");
}

TEST_P(InterpreterTest, Run_SkipOnly_WritesNothing) {
    nlohmann::json program = node::Skip();

    Run(program);

    EXPECT_EQ(output.str(), "");
}

TEST_P(InterpreterTest, Run_WhileWithFalseCondition_SkipsBody) {
    nlohmann::json program = node::Seq({
        node::While(node::Const(0), node::Write(node::Const(1))),
        node::Write(node::Const(2))
    });

    Run(program);

    EXPECT_EQ(output.str(), "2\n");
}

TEST_P(InterpreterTest, Run_DoWhileWithFalseCondition_ExecutesBodyOnce) {
    nlohmann::json program = node::DoWhile(node::Write(node::Const(7)), node::Const(0));

    Run(program);

    EXPECT_EQ(output.str(), "7\n");
}

TEST_P(InterpreterTest, Run_NegativeCondition_IsTreatedAsTrue) {
    nlohmann::json program = node::If(node::Const(-1), node::Write(node::Const(1)), node::Write(node::Const(0)));

    Run(program);

    EXPECT_EQ(output.str(), "1\n");
}

TEST_P(InterpreterTest, Run_LongLoop_CompletesAllIterations) {
    nlohmann::json program = node::Seq({
        node::Assign("i", node::Const(1)),
        node::Assign("s", node::Const(0)),
        node::While(node::BinOp("<=", node::Var("i"), node::Const(10000)),
                    node::Seq({
                        node::Assign("s", node::BinOp("+", node::Var("s"), node::Var("i"))),
                        node::Assign("i", node::BinOp("+", node::Var("i"), node::Const(1)))
                    })),
        node::Write(node::Var("s"))
    });

    Run(program);

    EXPECT_EQ(output.str(), "50005000\n");
}

TEST_P(InterpreterTest, Run_UndefinedVariableInUntakenBranch_DoesNotThrow) {
    nlohmann::json program = node::If(node::Const(0), node::Write(node::Var("missing")), node::Write(node::Const(1)));

    Run(program);

    EXPECT_EQ(output.str(), "1\n");
}

TEST_P(InterpreterTest, Run_UndefinedVariable_ThrowsUndefinedVariable) {
    nlohmann::json program = node::Write(node::Var("missing"));

    EXPECT_THROW(Run(program), UndefinedVariable);
}

TEST_P(InterpreterTest, Run_DivisionByZero_ThrowsDivisionByZero) {
    nlohmann::json program = node::Write(node::BinOp("/", node::Const(1), node::Const(0)));

    EXPECT_THROW(Run(program), DivisionByZero);
}

TEST_P(InterpreterTest, Run_ModuloByZero_ThrowsDivisionByZero) {
    nlohmann::json program = node::Write(node::BinOp("%", node::Const(1), node::Const(0)));

    EXPECT_THROW(Run(program), DivisionByZero);
}

TEST_P(InterpreterTest, Run_MissingInput_ThrowsInvalidInput) {
    nlohmann::json program = node::Read("x");

    EXPECT_THROW(Run(program), InvalidInput);
}

TEST_P(InterpreterTest, Run_NonNumericInput_ThrowsInvalidInput) {
    input.str("abc");
    nlohmann::json program = node::Read("x");

    EXPECT_THROW(Run(program), InvalidInput);
}

TEST_P(InterpreterTest, Run_ErrorAfterOutput_KeepsOutputBeforeError) {
    nlohmann::json program = node::Seq({
        node::Write(node::Const(1)),
        node::Write(node::BinOp("/", node::Const(1), node::Const(0))),
        node::Write(node::Const(2))
    });

    EXPECT_THROW(Run(program), DivisionByZero);
    EXPECT_EQ(output.str(), "1\n");
}

TEST_P(InterpreterTest, Run_InvalidJson_ThrowsSyntaxError) {
    std::string program = "{not-json}";

    EXPECT_THROW(Run(program), SyntaxError);
}

TEST_P(InterpreterTest, Run_UnknownStatement_ThrowsSyntaxError) {
    std::string program = R"({"jump": "start"})";

    EXPECT_THROW(Run(program), SyntaxError);
}

TEST_P(InterpreterTest, Run_UnknownOperator_ThrowsSyntaxError) {
    nlohmann::json program = node::Write(node::BinOp("^", node::Const(2), node::Const(3)));

    EXPECT_THROW(Run(program), SyntaxError);
}

TEST_P(InterpreterTest, Run_SyntaxErrorAfterValidStatements_WritesNothing) {
    nlohmann::json program = node::Seq({
        node::Write(node::Const(1)),
        node::Write(node::BinOp("^", node::Const(2), node::Const(3)))
    });

    EXPECT_THROW(Run(program), SyntaxError);
    EXPECT_EQ(output.str(), "");
}

INSTANTIATE_TEST_SUITE_P(Tree, InterpreterTest, ::testing::Values(&CreateTreeInterpreterFactory));
INSTANTIATE_TEST_SUITE_P(Bytecode, InterpreterTest, ::testing::Values(&CreateBytecodeInterpreterFactory));
