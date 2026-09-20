#pragma once

#include <string>
#include <string_view>
#include <unordered_map>

#include "parser/json/expression_parser/IJsonExpressionParser.hpp"

class JsonExpressionParserRegistry {
public:
    class Builder {
    public:
        Builder& Add(std::string_view key, IJsonExpressionParserPtr parser);

        JsonExpressionParserRegistry Build();

    private:
        std::unordered_map<std::string, IJsonExpressionParserPtr> parsers_;
    };

    IExprPtr Parse(const nlohmann::json& node, const JsonParser& parser) const;

private:
    explicit JsonExpressionParserRegistry(
        std::unordered_map<std::string, IJsonExpressionParserPtr> parsers);

    std::unordered_map<std::string, IJsonExpressionParserPtr> parsers_;
};