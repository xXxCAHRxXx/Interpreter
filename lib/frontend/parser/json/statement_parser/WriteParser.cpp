#include "WriteParser.hpp"

#include "frontend/ast/statement/Write.hpp"
#include "frontend/parser/json/JsonParser.hpp"
#include "frontend/parser/json/JsonUtils.hpp"

IStmtPtr WriteParser::Parse(const JsonParser& parser, const nlohmann::json& node) const {
    return std::make_shared<Write>(parser.ParseExpression(GetField(node, "write")));
}
