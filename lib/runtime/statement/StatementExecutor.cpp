#include "StatementExecutor.hpp"

#include "ast/statement/Assign.hpp"
#include "ast/statement/Block.hpp"
#include "ast/statement/DoWhile.hpp"
#include "ast/statement/If.hpp"
#include "ast/statement/Read.hpp"
#include "ast/statement/Skip.hpp"
#include "ast/statement/While.hpp"
#include "ast/statement/Write.hpp"

StatementExecutor::StatementExecutor(IValueStorePtr values, 
                      IExpressionEvaluatorFactoryPtr evaluator,
                      IInputPtr input,
                      IOutputPtr output)
: values_(std::move(values))
, evaluator_factory_(std::move(evaluator))
, input_(std::move(input))
, output_(std::move(output))
{ }

void StatementExecutor::Execute(IStmtPtr statement) {
    statement->Accept(*this);
}

void StatementExecutor::Visit(const Skip&) {
}

void StatementExecutor::Visit(const Assign& node) {
    values_->Set(node.GetName(), evaluate(node.GetValue()));
}

void StatementExecutor::Visit(const Read& node) {
    values_->Set(node.GetName(), input_->Read());
}

void StatementExecutor::Visit(const Write& node) {
    output_->Write(evaluate(node.GetExpr()));
}

void StatementExecutor::Visit(const If& node) {
    if (evaluate(node.GetCond())) {
        Execute(node.GetThenBranch());
    } else {
        Execute(node.GetElseBranch());
    }
}

void StatementExecutor::Visit(const While& node) {
    while (evaluate(node.GetCond())) {
        Execute(node.GetBody());
    }
}

void StatementExecutor::Visit(const DoWhile& node) {
    do {
        Execute(node.GetBody());
    } while (evaluate(node.GetCond()));
}

void StatementExecutor::Visit(const Block& node) {
    for (auto& statement : node.GetStatements()) {
        Execute(statement);
    }
}

Value StatementExecutor::evaluate(IExprPtr expr) const {
    IExpressionEvaluatorPtr evaluator = evaluator_factory_->Create(values_);

    return evaluator->Evaluate(expr);
}