#include "JsonParser.hpp"

#include <istream>
#include <string>
#include <utility>

#include "error/SyntaxError.hpp"

JsonParser::JsonParser(JsonStatementParserRegistry statements, JsonExpressionParserRegistry expressions)
: statements_(std::move(statements))
, expressions_(std::move(expressions))
{ }

IStmtPtr JsonParser::Parse(std::istream& input) const {
    nlohmann::json document;

    try {
        input >> document;
    } catch (const nlohmann::json::parse_error& error) {
        throw SyntaxError(std::string("invalid JSON: ") + error.what());
    }

    return ParseStatement(document);
}

IStmtPtr JsonParser::ParseStatement(const nlohmann::json& node) const {
    return statements_.Parse(node, *this);
}

IExprPtr JsonParser::ParseExpression(const nlohmann::json& node) const {
    return expressions_.Parse(node, *this);
}