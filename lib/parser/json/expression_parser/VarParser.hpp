#pragma once

#include "parser/json/expression_parser/IJsonExpressionParser.hpp"

class VarParser final : public IJsonExpressionParser {
public:
    IExprPtr Parse(const nlohmann::json& node, const JsonParser& parser) const override;
};