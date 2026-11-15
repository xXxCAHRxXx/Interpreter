#include "WhileParser.hpp"

#include "frontend/ast/statement/While.hpp"
#include "frontend/parser/json/JsonParser.hpp"
#include "frontend/parser/json/JsonUtils.hpp"

IStmtPtr WhileParser::Parse(const JsonParser& parser, const nlohmann::json& node) const {
    const nlohmann::json& body = GetField(node, "while");

    return std::make_shared<While>(parser.ParseExpression(GetField(body, "cond")),
                                   parser.ParseStatement(GetField(body, "body")));
}
