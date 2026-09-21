#pragma once

#include <memory>

#include <gsl/pointers>
#include <nlohmann/json.hpp>

#include "ast/statement/IStmt.hpp"

class JsonParser;

class IJsonStatementParser {
public:
    IJsonStatementParser(const IJsonStatementParser&) = delete;
    IJsonStatementParser& operator=(const IJsonStatementParser&) = delete;

    virtual IStmtPtr Parse(const nlohmann::json& node, const JsonParser& parser) const = 0;

    virtual ~IJsonStatementParser() = default;

protected:
    IJsonStatementParser() = default;
};

using IJsonStatementParserPtr = gsl::not_null<std::shared_ptr<IJsonStatementParser>>;