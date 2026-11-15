#include <iterator>
#include <memory>

#include <gtest/gtest.h>

#include "backend/bytecode/instruction/Label.hpp"
#include "backend/bytecode/instruction/WriteInstruction.hpp"
#include "backend/bytecode/program/Program.hpp"
#include "error/LogicError.hpp"

TEST(ProgramTest, Build_NoInstructions_ProgramIsEmpty) {
    Program::Builder builder;

    Program program = builder.Build();

    EXPECT_EQ(program.begin(), program.end());
}

TEST(ProgramTest, Build_ThreeInstructions_ProgramHasThreeInstructions) {
    Program::Builder builder;
    builder.AddInstruction(std::make_shared<WriteInstruction>())
           .AddInstruction(std::make_shared<WriteInstruction>())
           .AddInstruction(std::make_shared<WriteInstruction>());

    Program program = builder.Build();

    EXPECT_EQ(std::distance(program.begin(), program.end()), 3);
}

TEST(ProgramTest, Build_OnlyLabels_ProgramIsEmpty) {
    Program::Builder builder;
    builder.AddLabel(Label("start"))
           .AddLabel(Label("end"));

    Program program = builder.Build();

    EXPECT_EQ(program.begin(), program.end());
}

TEST(ProgramTest, AddLabel_SameLabelTwice_ThrowsLogicError) {
    Program::Builder builder;
    Label label("loop");
    builder.AddLabel(label);

    EXPECT_THROW(builder.AddLabel(label), LogicError);
}

TEST(ProgramTest, AddLabel_DifferentLabelsWithSameName_ThrowsLogicError) {
    Program::Builder builder;
    builder.AddLabel(Label("loop"));

    EXPECT_THROW(builder.AddLabel(Label("loop")), LogicError);
}

TEST(ProgramTest, AddLabel_TwoLabelsAtSamePosition_BothPointToSameInstruction) {
    Program::Builder builder;
    builder.AddInstruction(std::make_shared<WriteInstruction>())
           .AddLabel(Label("first"))
           .AddLabel(Label("second"))
           .AddInstruction(std::make_shared<WriteInstruction>());

    Program program = builder.Build();

    EXPECT_EQ(std::distance(program.begin(), program.Find(Label("first"))), 1);
    EXPECT_EQ(std::distance(program.begin(), program.Find(Label("second"))), 1);
}

TEST(ProgramTest, Find_UnknownLabel_ThrowsLogicError) {
    Program::Builder builder;
    builder.AddLabel(Label("known"))
           .AddInstruction(std::make_shared<WriteInstruction>());
    Program program = builder.Build();

    EXPECT_THROW(program.Find(Label("unknown")), LogicError);
}

TEST(ProgramTest, Find_LabelBeforeFirstInstruction_ReturnsBegin) {
    Program::Builder builder;
    builder.AddLabel(Label("start"))
           .AddInstruction(std::make_shared<WriteInstruction>());
    Program program = builder.Build();

    Program::Iterator position = program.Find(Label("start"));

    EXPECT_EQ(position, program.begin());
}

TEST(ProgramTest, Find_LabelAfterLastInstruction_ReturnsEnd) {
    Program::Builder builder;
    builder.AddInstruction(std::make_shared<WriteInstruction>())
           .AddLabel(Label("end"));
    Program program = builder.Build();

    Program::Iterator position = program.Find(Label("end"));

    EXPECT_EQ(position, program.end());
}

TEST(ProgramTest, Find_LabelInMiddle_ReturnsItsPosition) {
    Program::Builder builder;
    builder.AddInstruction(std::make_shared<WriteInstruction>())
           .AddInstruction(std::make_shared<WriteInstruction>())
           .AddLabel(Label("middle"))
           .AddInstruction(std::make_shared<WriteInstruction>());
    Program program = builder.Build();

    Program::Iterator position = program.Find(Label("middle"));

    EXPECT_EQ(std::distance(program.begin(), position), 2);
}
