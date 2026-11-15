#include <memory>
#include <sstream>
#include <utility>

#include <gtest/gtest.h>

#include "backend/bytecode/instruction/ConstantInstruction.hpp"
#include "backend/bytecode/instruction/Label.hpp"
#include "backend/bytecode/machine/ExecutionContext.hpp"
#include "backend/bytecode/program/Program.hpp"
#include "backend/runtime/environment/Environment.hpp"
#include "backend/runtime/io/StreamInput.hpp"
#include "backend/runtime/io/StreamOutput.hpp"
#include "error/InvalidInput.hpp"
#include "error/LogicError.hpp"
#include "error/StackUnderflow.hpp"
#include "error/UndefinedVariable.hpp"

class ExecutionContextTest : public ::testing::Test {
protected:
    ExecutionContext CreateContext(Program program) {
        return ExecutionContext(std::make_shared<const Program>(std::move(program)),
                                environment,
                                std::make_shared<StreamInput>(input),
                                std::make_shared<StreamOutput>(output));
    }

    std::shared_ptr<Environment> environment = std::make_shared<Environment>();
    std::istringstream input;
    std::ostringstream output;
};

TEST_F(ExecutionContextTest, HasNext_EmptyProgram_ReturnsFalse) {
    Program program = Program::Builder().Build();
    ExecutionContext context = CreateContext(std::move(program));

    bool has_next = context.HasNext();

    EXPECT_FALSE(has_next);
}

TEST_F(ExecutionContextTest, HasNext_NotStartedProgram_ReturnsTrue) {
    Program program = Program::Builder()
        .AddInstruction(std::make_shared<ConstantInstruction>(Value(1)))
        .Build();
    ExecutionContext context = CreateContext(std::move(program));

    bool has_next = context.HasNext();

    EXPECT_TRUE(has_next);
}

TEST_F(ExecutionContextTest, HasNext_AfterLastInstruction_ReturnsFalse) {
    Program program = Program::Builder()
        .AddInstruction(std::make_shared<ConstantInstruction>(Value(1)))
        .Build();
    ExecutionContext context = CreateContext(std::move(program));
    context.Next();

    bool has_next = context.HasNext();

    EXPECT_FALSE(has_next);
}

TEST_F(ExecutionContextTest, Next_TwoInstructions_ReturnsThemInProgramOrder) {
    Program program = Program::Builder()
        .AddInstruction(std::make_shared<ConstantInstruction>(Value(1)))
        .AddInstruction(std::make_shared<ConstantInstruction>(Value(2)))
        .Build();
    ExecutionContext context = CreateContext(std::move(program));

    context.Next()->Execute(context);
    context.Next()->Execute(context);

    EXPECT_EQ(context.PopFromStack(), Value(2));
    EXPECT_EQ(context.PopFromStack(), Value(1));
}

TEST_F(ExecutionContextTest, PopFromStack_EmptyStack_ThrowsStackUnderflow) {
    Program program = Program::Builder().Build();
    ExecutionContext context = CreateContext(std::move(program));

    EXPECT_THROW(context.PopFromStack(), StackUnderflow);
}

TEST_F(ExecutionContextTest, PopFromStack_AfterPush_ReturnsPushedValue) {
    Program program = Program::Builder().Build();
    ExecutionContext context = CreateContext(std::move(program));
    context.PushToStack(Value(5));

    Value value = context.PopFromStack();

    EXPECT_EQ(value, Value(5));
}

TEST_F(ExecutionContextTest, PopFromStack_SeveralPushes_ReturnsValuesInReverseOrder) {
    Program program = Program::Builder().Build();
    ExecutionContext context = CreateContext(std::move(program));
    context.PushToStack(Value(1));
    context.PushToStack(Value(2));
    context.PushToStack(Value(3));

    Value first = context.PopFromStack();
    Value second = context.PopFromStack();
    Value third = context.PopFromStack();

    EXPECT_EQ(first, Value(3));
    EXPECT_EQ(second, Value(2));
    EXPECT_EQ(third, Value(1));
}

TEST_F(ExecutionContextTest, PopFromStack_MorePopsThanPushes_ThrowsStackUnderflow) {
    Program program = Program::Builder().Build();
    ExecutionContext context = CreateContext(std::move(program));
    context.PushToStack(Value(1));
    context.PopFromStack();

    EXPECT_THROW(context.PopFromStack(), StackUnderflow);
}

TEST_F(ExecutionContextTest, LoadVariable_StoredVariable_ReturnsValue) {
    Program program = Program::Builder().Build();
    ExecutionContext context = CreateContext(std::move(program));
    context.StoreVariable("x", Value(9));

    Value value = context.LoadVariable("x");

    EXPECT_EQ(value, Value(9));
}

TEST_F(ExecutionContextTest, LoadVariable_UndefinedVariable_ThrowsUndefinedVariable) {
    Program program = Program::Builder().Build();
    ExecutionContext context = CreateContext(std::move(program));

    EXPECT_THROW(context.LoadVariable("x"), UndefinedVariable);
}

TEST_F(ExecutionContextTest, StoreVariable_ExistingVariable_OverwritesValue) {
    Program program = Program::Builder().Build();
    ExecutionContext context = CreateContext(std::move(program));
    context.StoreVariable("x", Value(1));

    context.StoreVariable("x", Value(2));

    EXPECT_EQ(environment->Get("x"), Value(2));
}

TEST_F(ExecutionContextTest, ReadInput_AvailableNumber_ReturnsValue) {
    input.str("42");
    Program program = Program::Builder().Build();
    ExecutionContext context = CreateContext(std::move(program));

    Value value = context.ReadInput();

    EXPECT_EQ(value, Value(42));
}

TEST_F(ExecutionContextTest, ReadInput_EmptyInput_ThrowsInvalidInput) {
    Program program = Program::Builder().Build();
    ExecutionContext context = CreateContext(std::move(program));

    EXPECT_THROW(context.ReadInput(), InvalidInput);
}

TEST_F(ExecutionContextTest, WriteOutput_Value_WritesValueToOutput) {
    Program program = Program::Builder().Build();
    ExecutionContext context = CreateContext(std::move(program));

    context.WriteOutput(Value(7));

    EXPECT_EQ(output.str(), "7\n");
}

TEST_F(ExecutionContextTest, Jump_UnknownLabel_ThrowsLogicError) {
    Program program = Program::Builder()
        .AddInstruction(std::make_shared<ConstantInstruction>(Value(1)))
        .Build();
    ExecutionContext context = CreateContext(std::move(program));

    EXPECT_THROW(context.Jump(Label("missing")), LogicError);
}

TEST_F(ExecutionContextTest, Jump_LabelAfterLastInstruction_FinishesProgram) {
    Program program = Program::Builder()
        .AddInstruction(std::make_shared<ConstantInstruction>(Value(1)))
        .AddLabel(Label("end"))
        .Build();
    ExecutionContext context = CreateContext(std::move(program));

    context.Jump(Label("end"));

    EXPECT_FALSE(context.HasNext());
}

TEST_F(ExecutionContextTest, Jump_BackwardLabel_ContinuesFromLabel) {
    Program program = Program::Builder()
        .AddLabel(Label("start"))
        .AddInstruction(std::make_shared<ConstantInstruction>(Value(7)))
        .Build();
    ExecutionContext context = CreateContext(std::move(program));
    context.Next();

    context.Jump(Label("start"));

    ASSERT_TRUE(context.HasNext());
    context.Next()->Execute(context);
    EXPECT_EQ(context.PopFromStack(), Value(7));
}
