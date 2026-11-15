#include "AssignParser.hpp"

#include "frontend/ast/statement/Assign.hpp"
#include "frontend/parser/json/JsonParser.hpp"
#include "frontend/parser/json/JsonUtils.hpp"

IStmtPtr AssignParser::Parse(const JsonParser& parser, const nlohmann::json& node) const {
    const nlohmann::json& body = GetField(node, "assn");

    return std::make_shared<Assign>(GetName(GetField(body, "dst")), parser.ParseExpression(GetField(body, "src")));
}
