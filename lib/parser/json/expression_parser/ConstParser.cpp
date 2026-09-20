#include "ConstParser.hpp"

#include <cstdint>

#include "ast/expression/Const.hpp"
#include "error/SyntaxError.hpp"
#include "parser/json/JsonUtils.hpp"

IExprPtr ConstParser::Parse(const nlohmann::json& node, const JsonParser&) const {
    const nlohmann::json& literal = GetField(node, "const");
    if (!literal.is_number_integer()) {
        throw SyntaxError("constant must be an integer");
    }

    return std::make_shared<Const>(Value(literal.get<std::int32_t>()));
}