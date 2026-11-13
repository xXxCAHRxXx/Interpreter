#include "VarParser.hpp"

#include "ast/expression/Var.hpp"
#include "parser/json/JsonUtils.hpp"

IExprPtr VarParser::Parse(const JsonParser&, const nlohmann::json& node) const {
    return std::make_shared<Var>(GetName(GetField(node, "var")));
}
