#pragma once

#include <memory>

#include <gsl/pointers>

#include "ast/expression/IExpr.hpp"
#include "value/Value.hpp"

class IExpressionEvaluator {
public:
    IExpressionEvaluator(const IExpressionEvaluator&) = delete;
    IExpressionEvaluator& operator=(const IExpressionEvaluator&) = delete;

    virtual Value Evaluate(IExprPtr expr) = 0;

    virtual ~IExpressionEvaluator() = default;

protected:
    IExpressionEvaluator() = default;
};

using IExpressionEvaluatorPtr = gsl::not_null<std::shared_ptr<IExpressionEvaluator>>;