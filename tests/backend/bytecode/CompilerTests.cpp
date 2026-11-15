#include <memory>
#include <sstream>

#include <gtest/gtest.h>

#include "TestSupport.hpp"
#include "backend/bytecode/compiler/Compiler.hpp"
#include "backend/bytecode/machine/Machine.hpp"
#include "backend/runtime/environment/Environment.hpp"
#include "backend/runtime/io/StreamInput.hpp"
#include "backend/runtime/io/StreamOutput.hpp"
#include "binary_operator/arithmetic/Add.hpp"
#include "binary_operator/arithmetic/Sub.hpp"
#include "binary_operator/comparison/Lt.hpp"
#include "error/UndefinedVariable.hpp"
#include "frontend/ast/statement/Assign.hpp"
#include "frontend/ast/statement/DoWhile.hpp"
#include "frontend/ast/statement/If.hpp"
#include "frontend/ast/statement/Read.hpp"
#include "frontend/ast/statement/Skip.hpp"
#include "frontend/ast/statement/While.hpp"
#include "frontend/ast/statement/Write.hpp"

class CompilerTest : public ::testing::Test {
protected:
    CompilerTest()
    : environment(std::make_shared<Environment>())
    , machine(environment, std::make_shared<StreamInput>(input), std::make_shared<StreamOutput>(output))
    { }

    std::istringstream input;
    std::ostringstream output;
    std::shared_ptr<Environment> environment;
    Machine machine;
    Compiler compiler;
};

TEST_F(CompilerTest, Compile_Skip_ProducesEmptyProgram) {
    auto statement = std::make_shared<Skip>();

    ProgramPtr program = compiler.Compile(statement);

    EXPECT_EQ(program->begin(), program->end());
}

TEST_F(CompilerTest, Compile_EmptyBlock_ProducesEmptyProgram) {
    IStmtPtr statement = Sequence({});

    ProgramPtr program = compiler.Compile(statement);

    EXPECT_EQ(program->begin(), program->end());
}

TEST_F(CompilerTest, Compile_WriteConstant_WritesConstant) {
    auto statement = std::make_shared<Write>(Constant(42));

    ProgramPtr program = compiler.Compile(statement);
    machine.Execute(program);

    EXPECT_EQ(output.str(), "42\n");
}

TEST_F(CompilerTest, Compile_AssignThenWrite_WritesAssignedValue) {
    IStmtPtr statement = Sequence({
        std::make_shared<Assign>("x", Binary<Add>(Constant(2), Constant(3))),
        std::make_shared<Write>(Variable("x"))
    });

    ProgramPtr program = compiler.Compile(statement);
    machine.Execute(program);

    EXPECT_EQ(output.str(), "5\n");
}

TEST_F(CompilerTest, Compile_NonCommutativeBinOp_KeepsOperandOrder) {
    auto statement = std::make_shared<Write>(Binary<Sub>(Constant(10), Constant(3)));

    ProgramPtr program = compiler.Compile(statement);
    machine.Execute(program);

    EXPECT_EQ(output.str(), "7\n");
}

TEST_F(CompilerTest, Compile_Read_StoresInputValue) {
    input.str("42");
    auto statement = std::make_shared<Read>("x");

    ProgramPtr program = compiler.Compile(statement);
    machine.Execute(program);

    EXPECT_EQ(environment->Get("x"), Value(42));
}

TEST_F(CompilerTest, Compile_UndefinedVariable_ThrowsOnlyOnExecution) {
    auto statement = std::make_shared<Write>(Variable("missing"));

    ProgramPtr program = compiler.Compile(statement);

    EXPECT_THROW(machine.Execute(program), UndefinedVariable);
}

TEST_F(CompilerTest, Compile_IfWithTrueCondition_ExecutesThenBranch) {
    auto statement = std::make_shared<If>(Constant(1), std::make_shared<Write>(Constant(1)), std::make_shared<Write>(Constant(2)));

    ProgramPtr program = compiler.Compile(statement);
    machine.Execute(program);

    EXPECT_EQ(output.str(), "1\n");
}

TEST_F(CompilerTest, Compile_IfWithFalseCondition_ExecutesElseBranch) {
    auto statement = std::make_shared<If>(Constant(0), std::make_shared<Write>(Constant(1)), std::make_shared<Write>(Constant(2)));

    ProgramPtr program = compiler.Compile(statement);
    machine.Execute(program);

    EXPECT_EQ(output.str(), "2\n");
}

TEST_F(CompilerTest, Compile_NestedIfs_ExecutesInnerElseBranch) {
    auto statement = std::make_shared<If>(
        Constant(1),
        std::make_shared<If>(Constant(0), std::make_shared<Write>(Constant(1)), std::make_shared<Write>(Constant(2))),
        std::make_shared<If>(Constant(1), std::make_shared<Write>(Constant(3)), std::make_shared<Write>(Constant(4)))
    );

    ProgramPtr program = compiler.Compile(statement);
    machine.Execute(program);

    EXPECT_EQ(output.str(), "2\n");
}

TEST_F(CompilerTest, Compile_WhileWithFalseCondition_SkipsBody) {
    auto statement = std::make_shared<While>(Constant(0), std::make_shared<Write>(Constant(1)));

    ProgramPtr program = compiler.Compile(statement);
    machine.Execute(program);

    EXPECT_EQ(output.str(), "");
}

TEST_F(CompilerTest, Compile_WhileCountingLoop_RepeatsBodyWhileConditionHolds) {
    environment->Set("i", Value(0));
    auto statement = std::make_shared<While>(
        Binary<Lt>(Variable("i"), Constant(3)),
        Sequence({
            std::make_shared<Write>(Variable("i")),
            std::make_shared<Assign>("i", Binary<Add>(Variable("i"), Constant(1)))
        })
    );

    ProgramPtr program = compiler.Compile(statement);
    machine.Execute(program);

    EXPECT_EQ(output.str(), "0\n1\n2\n");
}

TEST_F(CompilerTest, Compile_DoWhileWithFalseCondition_ExecutesBodyOnce) {
    auto statement = std::make_shared<DoWhile>(std::make_shared<Write>(Constant(7)), Constant(0));

    ProgramPtr program = compiler.Compile(statement);
    machine.Execute(program);

    EXPECT_EQ(output.str(), "7\n");
}

TEST_F(CompilerTest, Compile_DoWhileCountdown_RepeatsBodyWhileConditionHolds) {
    environment->Set("n", Value(3));
    auto statement = std::make_shared<DoWhile>(
        Sequence({
            std::make_shared<Write>(Variable("n")),
            std::make_shared<Assign>("n", Binary<Sub>(Variable("n"), Constant(1)))
        }),
        Variable("n")
    );

    ProgramPtr program = compiler.Compile(statement);
    machine.Execute(program);

    EXPECT_EQ(output.str(), "3\n2\n1\n");
}

TEST_F(CompilerTest, Compile_SameCompilerTwice_ProducesIndependentPrograms) {
    auto first = std::make_shared<If>(Constant(1), std::make_shared<Write>(Constant(1)), std::make_shared<Write>(Constant(2)));
    auto second = std::make_shared<If>(Constant(0), std::make_shared<Write>(Constant(3)), std::make_shared<Write>(Constant(4)));

    ProgramPtr first_program = compiler.Compile(first);
    ProgramPtr second_program = compiler.Compile(second);
    machine.Execute(first_program);
    machine.Execute(second_program);

    EXPECT_EQ(output.str(), "1\n4\n");
}
