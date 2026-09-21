#pragma once

#include <memory>

#include <gsl/pointers>

#include "ast/statement/IStmt.hpp"

class IStatementExecutor {
public:
    IStatementExecutor(const IStatementExecutor&) = delete;
    IStatementExecutor& operator=(const IStatementExecutor&) = delete;

    virtual void Execute(IStmtPtr statement) = 0;

    virtual ~IStatementExecutor() = default;

protected:
    IStatementExecutor() = default;
};

using IStatementExecutorPtr = gsl::not_null<std::shared_ptr<IStatementExecutor>>;