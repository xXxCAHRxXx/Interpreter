#include "ReadParser.hpp"

#include "ast/statement/Read.hpp"
#include "parser/json/JsonUtils.hpp"

IStmtPtr ReadParser::Parse(const JsonParser&, const nlohmann::json& node) const {
    return std::make_shared<Read>(GetName(GetField(node, "read")));
}