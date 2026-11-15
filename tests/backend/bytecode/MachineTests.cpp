#include <memory>
#include <sstream>

#include <gtest/gtest.h>

#include "backend/bytecode/instruction/BinaryOperationInstruction.hpp"
#include "backend/bytecode/instruction/ConstantInstruction.hpp"
#include "backend/bytecode/instruction/JumpInstruction.hpp"
#include "backend/bytecode/instruction/JumpNotZeroInstruction.hpp"
#include "backend/bytecode/instruction/Label.hpp"
#include "backend/bytecode/instruction/LoadInstruction.hpp"
#include "backend/bytecode/instruction/ReadInstruction.hpp"
#include "backend/bytecode/instruction/StoreInstruction.hpp"
#include "backend/bytecode/instruction/WriteInstruction.hpp"
#include "backend/bytecode/machine/Machine.hpp"
#include "backend/bytecode/program/Program.hpp"
#include "backend/runtime/environment/Environment.hpp"
#include "backend/runtime/io/StreamInput.hpp"
#include "backend/runtime/io/StreamOutput.hpp"
#include "binary_operator/arithmetic/Sub.hpp"
#include "error/LogicError.hpp"
#include "error/StackUnderflow.hpp"

class MachineTest : public ::testing::Test {
protected:
    MachineTest()
    : environment(std::make_shared<Environment>())
    , machine(environment, std::make_shared<StreamInput>(input), std::make_shared<StreamOutput>(output))
    { }

    std::istringstream input;
    std::ostringstream output;
    std::shared_ptr<Environment> environment;
    Machine machine;
};

TEST_F(MachineTest, Execute_EmptyProgram_WritesNothing) {
    auto program = std::make_shared<const Program>(Program::Builder().Build());

    machine.Execute(program);

    EXPECT_EQ(output.str(), "");
}

TEST_F(MachineTest, Execute_ConstantAndWrite_WritesConstant) {
    auto program = std::make_shared<const Program>(Program::Builder()
        .AddInstruction(std::make_shared<ConstantInstruction>(Value(42)))
        .AddInstruction(std::make_shared<WriteInstruction>())
        .Build());

    machine.Execute(program);

    EXPECT_EQ(output.str(), "42\n");
}

TEST_F(MachineTest, Execute_ReadStoreLoadWrite_EchoesInput) {
    input.str("17");
    auto program = std::make_shared<const Program>(Program::Builder()
        .AddInstruction(std::make_shared<ReadInstruction>())
        .AddInstruction(std::make_shared<StoreInstruction>("x"))
        .AddInstruction(std::make_shared<LoadInstruction>("x"))
        .AddInstruction(std::make_shared<WriteInstruction>())
        .Build());

    machine.Execute(program);

    EXPECT_EQ(output.str(), "17\n");
    EXPECT_EQ(environment->Get("x"), Value(17));
}

TEST_F(MachineTest, Execute_JumpOverInstructions_SkipsThem) {
    auto program = std::make_shared<const Program>(Program::Builder()
        .AddInstruction(std::make_shared<JumpInstruction>(Label("skip")))
        .AddInstruction(std::make_shared<ConstantInstruction>(Value(1)))
        .AddInstruction(std::make_shared<WriteInstruction>())
        .AddLabel(Label("skip"))
        .AddInstruction(std::make_shared<ConstantInstruction>(Value(2)))
        .AddInstruction(std::make_shared<WriteInstruction>())
        .Build());

    machine.Execute(program);

    EXPECT_EQ(output.str(), "2\n");
}

TEST_F(MachineTest, Execute_JumpToEnd_StopsExecution) {
    auto program = std::make_shared<const Program>(Program::Builder()
        .AddInstruction(std::make_shared<JumpInstruction>(Label("end")))
        .AddInstruction(std::make_shared<ConstantInstruction>(Value(1)))
        .AddInstruction(std::make_shared<WriteInstruction>())
        .AddLabel(Label("end"))
        .Build());

    machine.Execute(program);

    EXPECT_EQ(output.str(), "");
}

TEST_F(MachineTest, Execute_CountdownLoop_WritesEveryIteration) {
    environment->Set("n", Value(3));
    auto program = std::make_shared<const Program>(Program::Builder()
        .AddLabel(Label("loop"))
        .AddInstruction(std::make_shared<LoadInstruction>("n"))
        .AddInstruction(std::make_shared<WriteInstruction>())
        .AddInstruction(std::make_shared<LoadInstruction>("n"))
        .AddInstruction(std::make_shared<ConstantInstruction>(Value(1)))
        .AddInstruction(std::make_shared<BinaryOperationInstruction>(std::make_shared<Sub>()))
        .AddInstruction(std::make_shared<StoreInstruction>("n"))
        .AddInstruction(std::make_shared<LoadInstruction>("n"))
        .AddInstruction(std::make_shared<JumpNotZeroInstruction>(Label("loop")))
        .Build());

    machine.Execute(program);

    EXPECT_EQ(output.str(), "3\n2\n1\n");
}

TEST_F(MachineTest, Execute_SameProgramTwice_StartsFromBeginningEachTime) {
    auto program = std::make_shared<const Program>(Program::Builder()
        .AddInstruction(std::make_shared<ConstantInstruction>(Value(7)))
        .AddInstruction(std::make_shared<WriteInstruction>())
        .Build());

    machine.Execute(program);
    machine.Execute(program);

    EXPECT_EQ(output.str(), "7\n7\n");
}

TEST_F(MachineTest, Execute_WriteWithEmptyStack_ThrowsStackUnderflow) {
    auto program = std::make_shared<const Program>(Program::Builder()
        .AddInstruction(std::make_shared<WriteInstruction>())
        .Build());

    EXPECT_THROW(machine.Execute(program), StackUnderflow);
}

TEST_F(MachineTest, Execute_JumpToUnknownLabel_ThrowsLogicError) {
    auto program = std::make_shared<const Program>(Program::Builder()
        .AddInstruction(std::make_shared<JumpInstruction>(Label("missing")))
        .Build());

    EXPECT_THROW(machine.Execute(program), LogicError);
}
