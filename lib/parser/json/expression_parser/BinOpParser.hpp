#pragma once

#include "binary_operator/registry/BinaryOperatorRegistry.hpp"
#include "parser/json/expression_parser/IJsonExpressionParser.hpp"

class BinOpParser final : public IJsonExpressionParser {
public:
    explicit BinOpParser(BinaryOperatorRegistry operators);

    IExprPtr Parse(const JsonParser& parser, const nlohmann::json& node) const override;

private:
    BinaryOperatorRegistry operators_;
};
