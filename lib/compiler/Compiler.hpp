#pragma once

#include <cstddef>

#include "ICompiler.hpp"
#include "ast/expression/BinOp.hpp"
#include "ast/expression/Const.hpp"
#include "ast/expression/Var.hpp"
#include "ast/expression/visitor/IExprVisitor.hpp"
#include "ast/statement/Assign.hpp"
#include "ast/statement/Block.hpp"
#include "ast/statement/DoWhile.hpp"
#include "ast/statement/IStmt.hpp"
#include "ast/statement/If.hpp"
#include "ast/statement/Read.hpp"
#include "ast/statement/Skip.hpp"
#include "ast/statement/While.hpp"
#include "ast/statement/Write.hpp"
#include "ast/statement/visitor/IStmtVisitor.hpp"
#include "instruction/Label.hpp"
#include "instruction/Program.hpp"

class Compiler final : public ICompiler, public IExprVisitor, public IStmtVisitor {
public:
    Compiler();

    Program Compile(const IStmt& statement) override;

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
