#pragma once

#include "runtime/evaluator/ExpressionEvaluator.hpp"
#include "runtime/evaluator/factory/IExpressionEvaluatorFactory.hpp"

class ExpressionEvaluatorFactory : public IExpressionEvaluatorFactory {
public:
    IExpressionEvaluatorPtr Create(IValueReaderPtr values) const {
        return std::make_shared<ExpressionEvaluator>(std::move(values));
    }
};
