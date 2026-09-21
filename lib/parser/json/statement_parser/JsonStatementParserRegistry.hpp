#pragma once

#include <string>
#include <string_view>
#include <unordered_map>

#include "parser/json/statement_parser/IJsonStatementParser.hpp"

class JsonStatementParserRegistry {
public:
    class Builder {
    public:
        Builder& Add(std::string_view key, IJsonStatementParserPtr parser);

        JsonStatementParserRegistry Build();

    private:
        std::unordered_map<std::string, IJsonStatementParserPtr> parsers_;
    };

    IStmtPtr Parse(const nlohmann::json& node, const JsonParser& parser) const;

private:
    explicit JsonStatementParserRegistry(std::unordered_map<std::string, IJsonStatementParserPtr> parsers);

    std::unordered_map<std::string, IJsonStatementParserPtr> parsers_;
};