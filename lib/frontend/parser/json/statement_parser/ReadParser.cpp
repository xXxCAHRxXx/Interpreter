#include "ReadParser.hpp"

#include "frontend/ast/statement/Read.hpp"
#include "frontend/parser/json/JsonUtils.hpp"

IStmtPtr ReadParser::Parse(const JsonParser&, const nlohmann::json& node) const {
    return std::make_shared<Read>(GetName(GetField(node, "read")));
}
