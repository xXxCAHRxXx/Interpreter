#include "IfParser.hpp"

#include "ast/statement/If.hpp"
#include "parser/json/JsonParser.hpp"
#include "parser/json/JsonUtils.hpp"

IStmtPtr IfParser::Parse(const nlohmann::json& node, const JsonParser& parser) const {
    const nlohmann::json& body = GetField(node, "if");

    return std::make_shared<If>(parser.ParseExpression(GetField(body, "cond")),
                                parser.ParseStatement(GetField(body, "then")),
                                parser.ParseStatement(GetField(body, "else")));
}