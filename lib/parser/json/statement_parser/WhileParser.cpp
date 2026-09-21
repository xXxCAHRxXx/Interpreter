#include "WhileParser.hpp"

#include "ast/statement/While.hpp"
#include "parser/json/JsonParser.hpp"
#include "parser/json/JsonUtils.hpp"

IStmtPtr WhileParser::Parse(const nlohmann::json& node, const JsonParser& parser) const {
    const nlohmann::json& body = GetField(node, "while");

    return std::make_shared<While>(parser.ParseExpression(GetField(body, "cond")),
                                   parser.ParseStatement(GetField(body, "body")));
}