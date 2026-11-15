#pragma once

#include "frontend/parser/json/statement_parser/IJsonStatementParser.hpp"

class IfParser final : public IJsonStatementParser {
public:
    IStmtPtr Parse(const JsonParser& parser, const nlohmann::json& node) const override;
};
