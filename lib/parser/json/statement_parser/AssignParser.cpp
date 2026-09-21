#include "AssignParser.hpp"

#include "ast/statement/Assign.hpp"
#include "parser/json/JsonParser.hpp"
#include "parser/json/JsonUtils.hpp"

IStmtPtr AssignParser::Parse(const nlohmann::json& node, const JsonParser& parser) const {
    const nlohmann::json& body = GetField(node, "assn");

    return std::make_shared<Assign>(GetName(GetField(body, "dst")),
                                    parser.ParseExpression(GetField(body, "src")));
}