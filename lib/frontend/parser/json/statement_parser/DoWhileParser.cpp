#include "DoWhileParser.hpp"

#include "frontend/ast/statement/DoWhile.hpp"
#include "frontend/parser/json/JsonParser.hpp"
#include "frontend/parser/json/JsonUtils.hpp"

IStmtPtr DoWhileParser::Parse(const JsonParser& parser, const nlohmann::json& node) const {
    const nlohmann::json& body = GetField(node, "do");

    return std::make_shared<DoWhile>(parser.ParseStatement(GetField(body, "body")),
                                     parser.ParseExpression(GetField(body, "cond")));
}
