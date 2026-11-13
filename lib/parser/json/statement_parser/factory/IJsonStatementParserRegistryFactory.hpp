#pragma once

#include <memory>

#include <gsl/pointers>

#include "parser/json/statement_parser/JsonStatementParserRegistry.hpp"

class IJsonStatementParserRegistryFactory {
public:
    IJsonStatementParserRegistryFactory(const IJsonStatementParserRegistryFactory&) = delete;
    IJsonStatementParserRegistryFactory& operator=(const IJsonStatementParserRegistryFactory&) = delete;

    virtual JsonStatementParserRegistry Create() const = 0;

    virtual ~IJsonStatementParserRegistryFactory() = default;

protected:
    IJsonStatementParserRegistryFactory() = default;
};

using IJsonStatementParserRegistryFactoryPtr = gsl::not_null<std::shared_ptr<IJsonStatementParserRegistryFactory>>;
