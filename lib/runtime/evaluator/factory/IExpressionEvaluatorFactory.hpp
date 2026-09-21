#pragma once

#include <memory>

#include <gsl/pointers>

#include "runtime/environment/IValueReader.hpp"
#include "runtime/evaluator/IExpressionEvaluator.hpp"

class IExpressionEvaluatorFactory {
public:
    IExpressionEvaluatorFactory(const IExpressionEvaluatorFactory&) = delete;
    IExpressionEvaluatorFactory& operator=(const IExpressionEvaluatorFactory&) = delete;

    virtual IExpressionEvaluatorPtr Create(IValueReaderPtr values) const = 0;

    virtual ~IExpressionEvaluatorFactory() = default;

protected:
    IExpressionEvaluatorFactory() = default;
};

using IExpressionEvaluatorFactoryPtr = gsl::not_null<std::shared_ptr<IExpressionEvaluatorFactory>>;