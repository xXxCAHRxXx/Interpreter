#include <memory>

#include <gtest/gtest.h>
#include <nlohmann/json.hpp>

#include "TestSupport.hpp"
#include "error/LogicError.hpp"
#include "error/SyntaxError.hpp"
#include "frontend/ast/expression/Var.hpp"
#include "frontend/ast/statement/Read.hpp"
#include "frontend/ast/statement/Skip.hpp"
#include "frontend/parser/json/expression_parser/ConstParser.hpp"
#include "frontend/parser/json/expression_parser/JsonExpressionParserRegistry.hpp"
#include "frontend/parser/json/expression_parser/VarParser.hpp"
#include "frontend/parser/json/statement_parser/JsonStatementParserRegistry.hpp"
#include "frontend/parser/json/statement_parser/ReadParser.hpp"
#include "frontend/parser/json/statement_parser/SkipParser.hpp"

TEST(JsonStatementParserRegistryTest, Parse_RegisteredStringNode_UsesItsParser) {
    auto parser = CreateTestParser();
    JsonStatementParserRegistry registry = JsonStatementParserRegistry::Builder()
        .Add("skip", std::make_shared<SkipParser>())
        .Build();

    IStmtPtr statement = registry.Parse(*parser, nlohmann::json("skip"));

    EXPECT_NE(std::dynamic_pointer_cast<Skip>(statement.get()), nullptr);
}

TEST(JsonStatementParserRegistryTest, Parse_RegisteredObjectNode_UsesItsParser) {
    auto parser = CreateTestParser();
    JsonStatementParserRegistry registry = JsonStatementParserRegistry::Builder()
        .Add("read", std::make_shared<ReadParser>())
        .Build();

    IStmtPtr statement = registry.Parse(*parser, nlohmann::json{{"read", "n"}});

    EXPECT_NE(std::dynamic_pointer_cast<Read>(statement.get()), nullptr);
}

TEST(JsonStatementParserRegistryTest, Parse_UnregisteredNode_ThrowsSyntaxError) {
    auto parser = CreateTestParser();
    JsonStatementParserRegistry registry = JsonStatementParserRegistry::Builder()
        .Add("skip", std::make_shared<SkipParser>())
        .Build();

    EXPECT_THROW(registry.Parse(*parser, nlohmann::json{{"read", "n"}}), SyntaxError);
}

TEST(JsonStatementParserRegistryTest, Parse_EmptyRegistry_ThrowsSyntaxError) {
    auto parser = CreateTestParser();
    JsonStatementParserRegistry registry = JsonStatementParserRegistry::Builder().Build();

    EXPECT_THROW(registry.Parse(*parser, nlohmann::json("skip")), SyntaxError);
}

TEST(JsonStatementParserRegistryTest, Add_DuplicateKey_ThrowsLogicError) {
    JsonStatementParserRegistry::Builder builder;
    builder.Add("skip", std::make_shared<SkipParser>());

    EXPECT_THROW(builder.Add("skip", std::make_shared<SkipParser>()), LogicError);
}

TEST(JsonExpressionParserRegistryTest, Parse_RegisteredNode_UsesItsParser) {
    auto parser = CreateTestParser();
    JsonExpressionParserRegistry registry = JsonExpressionParserRegistry::Builder()
        .Add("var", std::make_shared<VarParser>())
        .Build();

    IExprPtr expression = registry.Parse(*parser, nlohmann::json{{"var", "x"}});

    EXPECT_NE(std::dynamic_pointer_cast<Var>(expression.get()), nullptr);
}

TEST(JsonExpressionParserRegistryTest, Parse_UnregisteredNode_ThrowsSyntaxError) {
    auto parser = CreateTestParser();
    JsonExpressionParserRegistry registry = JsonExpressionParserRegistry::Builder()
        .Add("var", std::make_shared<VarParser>())
        .Build();

    EXPECT_THROW(registry.Parse(*parser, nlohmann::json{{"const", 1}}), SyntaxError);
}

TEST(JsonExpressionParserRegistryTest, Parse_StringNode_ThrowsSyntaxError) {
    auto parser = CreateTestParser();
    JsonExpressionParserRegistry registry = JsonExpressionParserRegistry::Builder()
        .Add("var", std::make_shared<VarParser>())
        .Build();

    EXPECT_THROW(registry.Parse(*parser, nlohmann::json("var")), SyntaxError);
}

TEST(JsonExpressionParserRegistryTest, Add_DuplicateKey_ThrowsLogicError) {
    JsonExpressionParserRegistry::Builder builder;
    builder.Add("const", std::make_shared<ConstParser>());

    EXPECT_THROW(builder.Add("const", std::make_shared<ConstParser>()), LogicError);
}
