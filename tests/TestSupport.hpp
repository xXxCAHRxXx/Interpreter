#pragma once

#include <memory>
#include <utility>
#include <vector>

#include "binary_operator/registry/factory/BinaryOperatorRegistryFactory.hpp"
#include "parser/json/JsonParser.hpp"
#include "parser/json/expression_parser/factory/JsonExpressionParserRegistryFactory.hpp"
#include "parser/json/statement_parser/factory/JsonStatementParserRegistryFactory.hpp"
#include "runtime/io/IInput.hpp"
#include "runtime/io/IOutput.hpp"

inline std::shared_ptr<JsonParser> CreateTestParser() {
    JsonExpressionParserRegistryFactory expressions(std::make_shared<BinaryOperatorRegistryFactory>());

    return std::make_shared<JsonParser>(JsonStatementParserRegistryFactory().Create(), expressions.Create());
}

class StubInput final : public IInput {
public:
    explicit StubInput(std::vector<Value> values)
    : values_(std::move(values))
    { }

    Value Read() override {
        return values_.at(next_++);
    }

private:
    std::vector<Value> values_;
    std::size_t next_ = 0;
};

class RecordingOutput final : public IOutput {
public:
    void Write(const Value& value) override {
        values.push_back(value);
    }

    std::vector<Value> values;
};
