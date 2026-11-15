#include "IfParser.hpp"

#include "frontend/ast/statement/If.hpp"
#include "frontend/parser/json/JsonParser.hpp"
#include "frontend/parser/json/JsonUtils.hpp"

IStmtPtr IfParser::Parse(const JsonParser& parser, const nlohmann::json& node) const {
    const nlohmann::json& body = GetField(node, "if");

    return std::make_shared<If>(parser.ParseExpression(GetField(body, "cond")),
                                parser.ParseStatement(GetField(body, "then")),
                                parser.ParseStatement(GetField(body, "else")));
}
