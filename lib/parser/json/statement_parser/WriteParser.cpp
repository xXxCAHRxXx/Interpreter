#include "WriteParser.hpp"

#include "ast/statement/Write.hpp"
#include "parser/json/JsonParser.hpp"
#include "parser/json/JsonUtils.hpp"

IStmtPtr WriteParser::Parse(const nlohmann::json& node, const JsonParser& parser) const {
    return std::make_shared<Write>(parser.ParseExpression(GetField(node, "write")));
}