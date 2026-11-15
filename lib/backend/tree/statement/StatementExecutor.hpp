#pragma once

#include "IStatementExecutor.hpp"
#include "frontend/ast/expression/IExpr.hpp"
#include "frontend/ast/statement/IStmt.hpp"
#include "frontend/ast/statement/visitor/IStmtVisitor.hpp"
#include "backend/runtime/environment/IValueStore.hpp"
#include "backend/tree/evaluator/factory/IExpressionEvaluatorFactory.hpp"
#include "backend/runtime/io/IInput.hpp"
#include "backend/runtime/io/IOutput.hpp"
#include "value/Value.hpp"

class StatementExecutor final : public IStmtVisitor, public IStatementExecutor {
public:
    StatementExecutor(IValueStorePtr values,
                      IExpressionEvaluatorFactoryPtr evaluator_factory_,
                      IInputPtr input,
                      IOutputPtr output);

    void Execute(IStmtPtr statement) override;

    void Visit(const Skip& node) override;
    void Visit(const Assign& node) override;
    void Visit(const Read& node) override;
    void Visit(const Write& node) override;
    void Visit(const If& node) override;
    void Visit(const While& node) override;
    void Visit(const DoWhile& node) override;
    void Visit(const Block& node) override;

private:
    Value evaluate(IExprPtr expr) const;

    IValueStorePtr values_;
    IExpressionEvaluatorFactoryPtr evaluator_factory_;
    IInputPtr input_;
    IOutputPtr output_;
};
