#include <memory>
#include <sstream>

#include <gtest/gtest.h>

#include "backend/bytecode/instruction/BinaryOperationInstruction.hpp"
#include "backend/bytecode/instruction/ConstantInstruction.hpp"
#include "backend/bytecode/instruction/JumpInstruction.hpp"
#include "backend/bytecode/instruction/JumpNotZeroInstruction.hpp"
#include "backend/bytecode/instruction/JumpZeroInstruction.hpp"
#include "backend/bytecode/instruction/Label.hpp"
#include "backend/bytecode/instruction/LoadInstruction.hpp"
#include "backend/bytecode/instruction/ReadInstruction.hpp"
#include "backend/bytecode/instruction/StoreInstruction.hpp"
#include "backend/bytecode/instruction/WriteInstruction.hpp"
#include "backend/bytecode/machine/ExecutionContext.hpp"
#include "backend/bytecode/program/Program.hpp"
#include "backend/runtime/environment/Environment.hpp"
#include "backend/runtime/io/StreamInput.hpp"
#include "backend/runtime/io/StreamOutput.hpp"
#include "binary_operator/arithmetic/Div.hpp"
#include "binary_operator/arithmetic/Sub.hpp"
#include "error/DivisionByZero.hpp"
#include "error/InvalidInput.hpp"
#include "error/LogicError.hpp"
#include "error/StackUnderflow.hpp"
#include "error/UndefinedVariable.hpp"

class InstructionTest : public ::testing::Test {
protected:
    InstructionTest()
    : environment(std::make_shared<Environment>())
    , context(CreateProgram(),
              environment,
              std::make_shared<StreamInput>(input),
              std::make_shared<StreamOutput>(output))
    { }

    static ProgramPtr CreateProgram() {
        return std::make_shared<const Program>(Program::Builder()
            .AddInstruction(std::make_shared<ConstantInstruction>(Value(1)))
            .AddLabel(Label("target"))
            .AddInstruction(std::make_shared<ConstantInstruction>(Value(2)))
            .Build());
    }

    Value ExecuteNextInstruction() {
        context.Next()->Execute(context);

        return context.PopFromStack();
    }

    std::istringstream input;
    std::ostringstream output;
    std::shared_ptr<Environment> environment;
    ExecutionContext context;
};

using ConstantInstructionTest = InstructionTest;
using LoadInstructionTest = InstructionTest;
using StoreInstructionTest = InstructionTest;
using ReadInstructionTest = InstructionTest;
using WriteInstructionTest = InstructionTest;
using BinaryOperationInstructionTest = InstructionTest;
using JumpInstructionTest = InstructionTest;
using JumpZeroInstructionTest = InstructionTest;
using JumpNotZeroInstructionTest = InstructionTest;

TEST_F(ConstantInstructionTest, Execute_Value_PushesValue) {
    const ConstantInstruction instruction(Value(5));

    instruction.Execute(context);

    EXPECT_EQ(context.PopFromStack(), Value(5));
}

TEST_F(ConstantInstructionTest, Execute_Twice_PushesValueTwice) {
    const ConstantInstruction instruction(Value(5));

    instruction.Execute(context);
    instruction.Execute(context);

    EXPECT_EQ(context.PopFromStack(), Value(5));
    EXPECT_EQ(context.PopFromStack(), Value(5));
}

TEST_F(LoadInstructionTest, Execute_DefinedVariable_PushesVariableValue) {
    environment->Set("x", Value(9));
    const LoadInstruction instruction("x");

    instruction.Execute(context);

    EXPECT_EQ(context.PopFromStack(), Value(9));
}

TEST_F(LoadInstructionTest, Execute_UndefinedVariable_ThrowsUndefinedVariable) {
    const LoadInstruction instruction("x");

    EXPECT_THROW(instruction.Execute(context), UndefinedVariable);
}

TEST_F(StoreInstructionTest, Execute_ValueOnStack_StoresVariable) {
    context.PushToStack(Value(4));
    const StoreInstruction instruction("x");

    instruction.Execute(context);

    EXPECT_EQ(environment->Get("x"), Value(4));
}

TEST_F(StoreInstructionTest, Execute_ValueOnStack_PopsValue) {
    context.PushToStack(Value(4));
    const StoreInstruction instruction("x");

    instruction.Execute(context);

    EXPECT_THROW(context.PopFromStack(), StackUnderflow);
}

TEST_F(StoreInstructionTest, Execute_EmptyStack_ThrowsStackUnderflow) {
    const StoreInstruction instruction("x");

    EXPECT_THROW(instruction.Execute(context), StackUnderflow);
}

TEST_F(ReadInstructionTest, Execute_AvailableInput_PushesInputValue) {
    input.str("42");
    const ReadInstruction instruction;

    instruction.Execute(context);

    EXPECT_EQ(context.PopFromStack(), Value(42));
}

TEST_F(ReadInstructionTest, Execute_EmptyInput_ThrowsInvalidInput) {
    const ReadInstruction instruction;

    EXPECT_THROW(instruction.Execute(context), InvalidInput);
}

TEST_F(WriteInstructionTest, Execute_ValueOnStack_WritesValue) {
    context.PushToStack(Value(7));
    const WriteInstruction instruction;

    instruction.Execute(context);

    EXPECT_EQ(output.str(), "7\n");
}

TEST_F(WriteInstructionTest, Execute_ValueOnStack_PopsValue) {
    context.PushToStack(Value(7));
    const WriteInstruction instruction;

    instruction.Execute(context);

    EXPECT_THROW(context.PopFromStack(), StackUnderflow);
}

TEST_F(WriteInstructionTest, Execute_EmptyStack_ThrowsStackUnderflow) {
    const WriteInstruction instruction;

    EXPECT_THROW(instruction.Execute(context), StackUnderflow);
}

TEST_F(BinaryOperationInstructionTest, Execute_TwoValues_PushesResultInOperandOrder) {
    context.PushToStack(Value(10));
    context.PushToStack(Value(3));
    const BinaryOperationInstruction instruction(std::make_shared<Sub>());

    instruction.Execute(context);

    EXPECT_EQ(context.PopFromStack(), Value(7));
}

TEST_F(BinaryOperationInstructionTest, Execute_OneValue_ThrowsStackUnderflow) {
    context.PushToStack(Value(10));
    const BinaryOperationInstruction instruction(std::make_shared<Sub>());

    EXPECT_THROW(instruction.Execute(context), StackUnderflow);
}

TEST_F(BinaryOperationInstructionTest, Execute_ZeroDivisor_ThrowsDivisionByZero) {
    context.PushToStack(Value(10));
    context.PushToStack(Value(0));
    const BinaryOperationInstruction instruction(std::make_shared<Div>());

    EXPECT_THROW(instruction.Execute(context), DivisionByZero);
}

TEST_F(JumpInstructionTest, Execute_KnownLabel_ContinuesFromLabel) {
    const JumpInstruction instruction(Label("target"));

    instruction.Execute(context);

    EXPECT_EQ(ExecuteNextInstruction(), Value(2));
}

TEST_F(JumpInstructionTest, Execute_UnknownLabel_ThrowsLogicError) {
    const JumpInstruction instruction(Label("missing"));

    EXPECT_THROW(instruction.Execute(context), LogicError);
}

TEST_F(JumpZeroInstructionTest, Execute_Zero_ContinuesFromLabel) {
    context.PushToStack(Value(0));
    const JumpZeroInstruction instruction(Label("target"));

    instruction.Execute(context);

    EXPECT_EQ(ExecuteNextInstruction(), Value(2));
}

TEST_F(JumpZeroInstructionTest, Execute_NonZero_ContinuesWithNextInstruction) {
    context.PushToStack(Value(1));
    const JumpZeroInstruction instruction(Label("target"));

    instruction.Execute(context);

    EXPECT_EQ(ExecuteNextInstruction(), Value(1));
}

TEST_F(JumpZeroInstructionTest, Execute_Negative_ContinuesWithNextInstruction) {
    context.PushToStack(Value(-1));
    const JumpZeroInstruction instruction(Label("target"));

    instruction.Execute(context);

    EXPECT_EQ(ExecuteNextInstruction(), Value(1));
}

TEST_F(JumpZeroInstructionTest, Execute_EmptyStack_ThrowsStackUnderflow) {
    const JumpZeroInstruction instruction(Label("target"));

    EXPECT_THROW(instruction.Execute(context), StackUnderflow);
}

TEST_F(JumpNotZeroInstructionTest, Execute_NonZero_ContinuesFromLabel) {
    context.PushToStack(Value(5));
    const JumpNotZeroInstruction instruction(Label("target"));

    instruction.Execute(context);

    EXPECT_EQ(ExecuteNextInstruction(), Value(2));
}

TEST_F(JumpNotZeroInstructionTest, Execute_Zero_ContinuesWithNextInstruction) {
    context.PushToStack(Value(0));
    const JumpNotZeroInstruction instruction(Label("target"));

    instruction.Execute(context);

    EXPECT_EQ(ExecuteNextInstruction(), Value(1));
}

TEST_F(JumpNotZeroInstructionTest, Execute_EmptyStack_ThrowsStackUnderflow) {
    const JumpNotZeroInstruction instruction(Label("target"));

    EXPECT_THROW(instruction.Execute(context), StackUnderflow);
}
