#pragma once

#include <vector>

#include "parser/json/statement_parser/IJsonStatementParser.hpp"

class SeqParser final : public IJsonStatementParser {
public:
    IStmtPtr Parse(const nlohmann::json& node, const JsonParser& parser) const override;

private:
    static void collect(const nlohmann::json& node,
                        const JsonParser& parser,
                        std::vector<IStmtPtr>& statements);
};