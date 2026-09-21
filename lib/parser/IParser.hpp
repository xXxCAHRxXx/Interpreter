#pragma once

#include <iosfwd>
#include <memory>

#include <gsl/pointers>

#include "ast/statement/IStmt.hpp"

class IParser {
public:
    IParser(const IParser&) = delete;
    IParser& operator=(const IParser&) = delete;

    virtual IStmtPtr Parse(std::istream& input) const = 0;

    virtual ~IParser() = default;

protected:
    IParser() = default;
};

using IParserPtr = gsl::not_null<std::shared_ptr<IParser>>;