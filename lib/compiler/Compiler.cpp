#include "Compiler.hpp"

#include <memory>
#include <string>

#include "instruction/BinaryOperationInstruction.hpp"
#include "instruction/ConstantInstruction.hpp"
#include "instruction/JumpInstruction.hpp"
#include "instruction/JumpNotZeroInstruction.hpp"
#include "instruction/JumpZeroInstruction.hpp"
#include "instruction/LoadInstruction.hpp"
#include "instruction/ReadInstruction.hpp"
#include "instruction/StoreInstruction.hpp"
#include "instruction/WriteInstruction.hpp"

Compiler::Compiler()
: label_count_(0) { }

Program Compiler::Compile(const IStmt& statement) {
    builder_ = Program::Builder();
    label_count_ = 0;

    statement.Accept(*this);

    return builder_.Build();
}

void Compiler::Visit(const Var& node) {
    builder_.AddInstruction(std::make_shared<LoadInstruction>(node.GetName()));
}

void Compiler::Visit(const Const& node) {
    builder_.AddInstruction(std::make_shared<ConstantInstruction>(node.GetValue()));
}

void Compiler::Visit(const BinOp& node) {
    node.GetLhs()->Accept(*this);
    node.GetRhs()->Accept(*this);
    builder_.AddInstruction(std::make_shared<BinaryOperationInstruction>(node.GetOperator()));
}

void Compiler::Visit(const Skip&) { }

void Compiler::Visit(const Assign& node) {
    node.GetValue()->Accept(*this);
    builder_.AddInstruction(std::make_shared<StoreInstruction>(node.GetName()));
}

void Compiler::Visit(const Read& node) {
    builder_.AddInstruction(std::make_shared<ReadInstruction>());
    builder_.AddInstruction(std::make_shared<StoreInstruction>(node.GetName()));
}

void Compiler::Visit(const Write& node) {
    node.GetExpr()->Accept(*this);
    builder_.AddInstruction(std::make_shared<WriteInstruction>());
}

void Compiler::Visit(const If& node) {
    Label else_label = NewLabel();
    Label end_label = NewLabel();

    node.GetCond()->Accept(*this);
    builder_.AddInstruction(std::make_shared<JumpZeroInstruction>(else_label));
    node.GetThenBranch()->Accept(*this);
    builder_.AddInstruction(std::make_shared<JumpInstruction>(end_label));
    builder_.AddLabel(else_label);
    node.GetElseBranch()->Accept(*this);
    builder_.AddLabel(end_label);
}

void Compiler::Visit(const While& node) {
    Label start_label = NewLabel();
    Label end_label = NewLabel();

    builder_.AddLabel(start_label);
    node.GetCond()->Accept(*this);
    builder_.AddInstruction(std::make_shared<JumpZeroInstruction>(end_label));
    node.GetBody()->Accept(*this);
    builder_.AddInstruction(std::make_shared<JumpInstruction>(start_label));
    builder_.AddLabel(end_label);
}

void Compiler::Visit(const DoWhile& node) {
    Label start_label = NewLabel();

    builder_.AddLabel(start_label);
    node.GetBody()->Accept(*this);
    node.GetCond()->Accept(*this);
    builder_.AddInstruction(std::make_shared<JumpNotZeroInstruction>(start_label));
}

void Compiler::Visit(const Block& node) {
    for (auto statement : node.GetStatements()) {
        statement->Accept(*this);
    }
}

Label Compiler::NewLabel() {
    return Label("L" + std::to_string(label_count_++));
}
