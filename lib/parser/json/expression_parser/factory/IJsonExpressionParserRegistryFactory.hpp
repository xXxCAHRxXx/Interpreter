#pragma once

#include <memory>

#include <gsl/pointers>

#include "parser/json/expression_parser/JsonExpressionParserRegistry.hpp"

class IJsonExpressionParserRegistryFactory {
public:
    IJsonExpressionParserRegistryFactory(const IJsonExpressionParserRegistryFactory&) = delete;
    IJsonExpressionParserRegistryFactory& operator=(const IJsonExpressionParserRegistryFactory&) = delete;

    virtual JsonExpressionParserRegistry Create() const = 0;

    virtual ~IJsonExpressionParserRegistryFactory() = default;

protected:
    IJsonExpressionParserRegistryFactory() = default;
};

using IJsonExpressionParserRegistryFactoryPtr = gsl::not_null<std::shared_ptr<IJsonExpressionParserRegistryFactory>>;
