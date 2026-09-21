#pragma once

#include "parser/json/statement_parser/IJsonStatementParser.hpp"

class IfParser final : public IJsonStatementParser {
public:
    IStmtPtr Parse(const nlohmann::json& node, const JsonParser& parser) const override;
};