#pragma once

#include "ast/expression/IExpr.hpp"
#include "ast/statement/IStmt.hpp"
#include "ast/statement/visitor/IStmtVisitor.hpp"
#include "runtime/environment/IValueStore.hpp"
#include "runtime/evaluator/factory/IExpressionEvaluatorFactory.hpp"
#include "runtime/io/IInput.hpp"
#include "runtime/io/IOutput.hpp"
#include "IStatementExecutor.hpp"
#include "value/Value.hpp"

class StatementExecutor final : public IStmtVisitor, public IStatementExecutor {
public:
    StatementExecutor(IValueStorePtr values, 
                      IExpressionEvaluatorFactoryPtr evaluator_factory_,
                      IInputPtr input,
                      IOutputPtr output);

    void Execute(IStmtPtr statement);

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