#pragma once

#include "frontend/parser/json/expression_parser/IJsonExpressionParser.hpp"

class ConstParser final : public IJsonExpressionParser {
public:
    IExprPtr Parse(const JsonParser& parser, const nlohmann::json& node) const override;
};
