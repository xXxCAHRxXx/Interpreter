#pragma once

#include <nlohmann/json.hpp>

#include "ast/expression/IExpr.hpp"
#include "ast/statement/IStmt.hpp"
#include "parser/IParser.hpp"
#include "parser/json/expression_parser/JsonExpressionParserRegistry.hpp"
#include "parser/json/statement_parser/JsonStatementParserRegistry.hpp"

class JsonParser final : public IParser {
public:
    JsonParser(JsonStatementParserRegistry statements, JsonExpressionParserRegistry expressions);

    IStmtPtr Parse(std::istream& input) const override;

    IStmtPtr ParseStatement(const nlohmann::json& node) const;
    IExprPtr ParseExpression(const nlohmann::json& node) const;

private:
    JsonStatementParserRegistry statements_;
    JsonExpressionParserRegistry expressions_;
};