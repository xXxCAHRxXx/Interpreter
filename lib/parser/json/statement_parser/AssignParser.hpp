#pragma once

#include "parser/json/statement_parser/IJsonStatementParser.hpp"

class AssignParser final : public IJsonStatementParser {
public:
    IStmtPtr Parse(const JsonParser& parser, const nlohmann::json& node) const override;
};