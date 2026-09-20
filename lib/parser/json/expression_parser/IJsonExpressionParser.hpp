#pragma once

#include <memory>

#include <gsl/pointers>
#include <nlohmann/json.hpp>

#include "ast/expression/IExpr.hpp"

class JsonParser;

class IJsonExpressionParser {
public:
    IJsonExpressionParser(const IJsonExpressionParser&) = delete;
    IJsonExpressionParser& operator=(const IJsonExpressionParser&) = delete;

    virtual IExprPtr Parse(const nlohmann::json& node, const JsonParser& parser) const = 0;

    virtual ~IJsonExpressionParser() = default;

protected:
    IJsonExpressionParser() = default;
};

using IJsonExpressionParserPtr = gsl::not_null<std::shared_ptr<IJsonExpressionParser>>;