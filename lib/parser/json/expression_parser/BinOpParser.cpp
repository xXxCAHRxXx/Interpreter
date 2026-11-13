#include "BinOpParser.hpp"

#include <utility>

#include "ast/expression/BinOp.hpp"
#include "parser/json/JsonParser.hpp"
#include "parser/json/JsonUtils.hpp"

BinOpParser::BinOpParser(BinaryOperatorRegistry operators)
: operators_(std::move(operators)) { }

IExprPtr BinOpParser::Parse(const JsonParser& parser, const nlohmann::json& node) const {
    return std::make_shared<BinOp>(operators_.Get(GetName(GetField(node, "binop"))),
                                   parser.ParseExpression(GetField(node, "left")),
                                   parser.ParseExpression(GetField(node, "right")));
}
