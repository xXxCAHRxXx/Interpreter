#pragma once

#include "IJsonExpressionParserRegistryFactory.hpp"
#include "binary_operator/registry/factory/IBinaryOperatorRegistryFactory.hpp"

class JsonExpressionParserRegistryFactory final : public IJsonExpressionParserRegistryFactory {
public:
    explicit JsonExpressionParserRegistryFactory(IBinaryOperatorRegistryFactoryPtr operators);

    JsonExpressionParserRegistry Create() const override;

private:
    IBinaryOperatorRegistryFactoryPtr operators_;
};
