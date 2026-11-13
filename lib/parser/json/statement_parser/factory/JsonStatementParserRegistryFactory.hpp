#pragma once

#include "IJsonStatementParserRegistryFactory.hpp"

class JsonStatementParserRegistryFactory final : public IJsonStatementParserRegistryFactory {
public:
    JsonStatementParserRegistry Create() const override;
};
