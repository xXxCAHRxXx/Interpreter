#pragma once

#include <memory>
#include <utility>
#include <vector>

#include "binary_operator/Add.hpp"
#include "binary_operator/And.hpp"
#include "binary_operator/BinaryOperatorRegistry.hpp"
#include "binary_operator/Div.hpp"
#include "binary_operator/Eq.hpp"
#include "binary_operator/Ge.hpp"
#include "binary_operator/Gt.hpp"
#include "binary_operator/Le.hpp"
#include "binary_operator/Lt.hpp"
#include "binary_operator/Mod.hpp"
#include "binary_operator/Mul.hpp"
#include "binary_operator/Ne.hpp"
#include "binary_operator/Or.hpp"
#include "binary_operator/Sub.hpp"
#include "parser/json/JsonParser.hpp"
#include "parser/json/expression_parser/BinOpParser.hpp"
#include "parser/json/expression_parser/ConstParser.hpp"
#include "parser/json/expression_parser/VarParser.hpp"
#include "parser/json/statement_parser/AssignParser.hpp"
#include "parser/json/statement_parser/DoWhileParser.hpp"
#include "parser/json/statement_parser/IfParser.hpp"
#include "parser/json/statement_parser/ReadParser.hpp"
#include "parser/json/statement_parser/SeqParser.hpp"
#include "parser/json/statement_parser/SkipParser.hpp"
#include "parser/json/statement_parser/WhileParser.hpp"
#include "parser/json/statement_parser/WriteParser.hpp"
#include "runtime/io/IInput.hpp"
#include "runtime/io/IOutput.hpp"

inline BinaryOperatorRegistryPtr CreateTestOperators() {
    BinaryOperatorRegistry::Builder builder;

    builder.Add("+", std::make_shared<Add>())
           .Add("-", std::make_shared<Sub>())
           .Add("*", std::make_shared<Mul>())
           .Add("/", std::make_shared<Div>())
           .Add("%", std::make_shared<Mod>())
           .Add("<", std::make_shared<Lt>())
           .Add("<=", std::make_shared<Le>())
           .Add(">", std::make_shared<Gt>())
           .Add(">=", std::make_shared<Ge>())
           .Add("==", std::make_shared<Eq>())
           .Add("!=", std::make_shared<Ne>())
           .Add("&&", std::make_shared<And>())
           .Add("!!", std::make_shared<Or>());

    return std::make_shared<BinaryOperatorRegistry>(builder.Build());
}

inline std::shared_ptr<JsonParser> CreateTestParser() {
    JsonStatementParserRegistry::Builder statement_builder;
    statement_builder.Add("skip", std::make_shared<SkipParser>())
                     .Add("assn", std::make_shared<AssignParser>())
                     .Add("read", std::make_shared<ReadParser>())
                     .Add("write", std::make_shared<WriteParser>())
                     .Add("if", std::make_shared<IfParser>())
                     .Add("while", std::make_shared<WhileParser>())
                     .Add("do", std::make_shared<DoWhileParser>())
                     .Add("seq", std::make_shared<SeqParser>());

    JsonExpressionParserRegistry::Builder expression_builder;
    expression_builder.Add("var", std::make_shared<VarParser>())
                      .Add("const", std::make_shared<ConstParser>())
                      .Add("binop", std::make_shared<BinOpParser>(CreateTestOperators()));

    return std::make_shared<JsonParser>(statement_builder.Build(), expression_builder.Build());
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
