#pragma once

#include "backend/tree/evaluator/ExpressionEvaluator.hpp"
#include "backend/tree/evaluator/factory/IExpressionEvaluatorFactory.hpp"

class ExpressionEvaluatorFactory : public IExpressionEvaluatorFactory {
public:
    IExpressionEvaluatorPtr Create(IValueReaderPtr values) const override {
        return std::make_shared<ExpressionEvaluator>(std::move(values));
    }
};
