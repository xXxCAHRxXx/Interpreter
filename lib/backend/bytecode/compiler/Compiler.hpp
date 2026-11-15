#pragma once

#include <cstddef>

#include "ICompiler.hpp"
#include "frontend/ast/expression/BinOp.hpp"
#include "frontend/ast/expression/Const.hpp"
#include "frontend/ast/expression/Var.hpp"
#include "frontend/ast/expression/visitor/IExprVisitor.hpp"
#include "frontend/ast/statement/Assign.hpp"
#include "frontend/ast/statement/Block.hpp"
#include "frontend/ast/statement/DoWhile.hpp"
#include "frontend/ast/statement/IStmt.hpp"
#include "frontend/ast/statement/If.hpp"
#include "frontend/ast/statement/Read.hpp"
#include "frontend/ast/statement/Skip.hpp"
#include "frontend/ast/statement/While.hpp"
#include "frontend/ast/statement/Write.hpp"
#include "frontend/ast/statement/visitor/IStmtVisitor.hpp"
#include "backend/bytecode/instruction/Label.hpp"
#include "backend/bytecode/program/Program.hpp"

class Compiler final : public ICompiler, public IExprVisitor, public IStmtVisitor {
public:
    Compiler();

    ProgramPtr Compile(IStmtPtr statement) override;

    void Visit(const Var& node) override;
    void Visit(const Const& node) override;
    void Visit(const BinOp& node) override;

    void Visit(const Skip& node) override;
    void Visit(const Assign& node) override;
    void Visit(const Read& node) override;
    void Visit(const Write& node) override;
    void Visit(const If& node) override;
    void Visit(const While& node) override;
    void Visit(const DoWhile& node) override;
    void Visit(const Block& node) override;

private:
    Label NewLabel();

    Program::Builder builder_;
    std::size_t label_count_;
};
