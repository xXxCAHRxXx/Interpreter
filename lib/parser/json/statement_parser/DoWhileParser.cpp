#include "DoWhileParser.hpp"

#include "ast/statement/DoWhile.hpp"
#include "parser/json/JsonParser.hpp"
#include "parser/json/JsonUtils.hpp"

IStmtPtr DoWhileParser::Parse(const JsonParser& parser, const nlohmann::json& node) const {
    const nlohmann::json& body = GetField(node, "do");

    return std::make_shared<DoWhile>(parser.ParseStatement(GetField(body, "body")),
                                     parser.ParseExpression(GetField(body, "cond")));
}