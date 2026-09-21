#pragma once

#include "binary_operator/BinaryOperatorRegistry.hpp"
#include "parser/json/expression_parser/IJsonExpressionParser.hpp"

class BinOpParser final : public IJsonExpressionParser {
public:
    explicit BinOpParser(BinaryOperatorRegistryPtr operators);

    IExprPtr Parse(const nlohmann::json& node, const JsonParser& parser) const override;

private:
    BinaryOperatorRegistryPtr operators_;
};