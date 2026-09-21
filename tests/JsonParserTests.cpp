#include <memory>
#include <sstream>

#include <gtest/gtest.h>

#include "TestSupport.hpp"
#include "ast/expression/BinOp.hpp"
#include "ast/expression/Const.hpp"
#include "ast/expression/Var.hpp"
#include "ast/statement/Assign.hpp"
#include "ast/statement/Block.hpp"
#include "ast/statement/Skip.hpp"
#include "ast/statement/Write.hpp"
#include "error/SyntaxError.hpp"

TEST(JsonParserTest, ParsesSkipFromString) {
    auto parser = CreateTestParser();
    std::istringstream program(R"("skip")");

    auto statement = parser->Parse(program);

    EXPECT_NE(std::dynamic_pointer_cast<Skip>(statement.get()), nullptr);
}

TEST(JsonParserTest, FlattensNestedSequencesIntoBlock) {
    auto parser = CreateTestParser();
    std::istringstream program(R"(
        {
          "seq": {
            "left": { "assn": { "dst": "x", "src": { "const": 1 } } },
            "right": {
              "seq": {
                "left": { "write": { "var": "x" } },
                "right": "skip"
              }
            }
          }
        }
    )");

    auto statement = parser->Parse(program);
    auto block = std::dynamic_pointer_cast<Block>(statement.get());

    ASSERT_NE(block, nullptr);
    ASSERT_EQ(block->GetStatements().size(), 3U);
    EXPECT_NE(std::dynamic_pointer_cast<Assign>(block->GetStatements()[0].get()), nullptr);
    EXPECT_NE(std::dynamic_pointer_cast<Write>(block->GetStatements()[1].get()), nullptr);
    EXPECT_NE(std::dynamic_pointer_cast<Skip>(block->GetStatements()[2].get()), nullptr);
}

TEST(JsonParserTest, BuildsBinaryExpressionTree) {
    auto parser = CreateTestParser();
    const nlohmann::json node = {
        {"binop", "+"},
        {"left", {{"const", 2}}},
        {"right", {{"var", "x"}}}
    };

    auto expression = parser->ParseExpression(node);
    auto binary = std::dynamic_pointer_cast<BinOp>(expression.get());

    ASSERT_NE(binary, nullptr);
    EXPECT_NE(std::dynamic_pointer_cast<Const>(binary->GetLhs().get()), nullptr);
    EXPECT_NE(std::dynamic_pointer_cast<Var>(binary->GetRhs().get()), nullptr);
}

TEST(JsonParserTest, RejectsInvalidJson) {
    auto parser = CreateTestParser();
    std::istringstream program("{not-json}");

    EXPECT_THROW(parser->Parse(program), SyntaxError);
}

TEST(JsonParserTest, RejectsUnknownStatementAndExpression) {
    auto parser = CreateTestParser();
    std::istringstream program(R"({"unknown": 1})");

    EXPECT_THROW(parser->Parse(program), SyntaxError);
    EXPECT_THROW(parser->ParseExpression(nlohmann::json{{"unknown", 1}}), SyntaxError);
}
