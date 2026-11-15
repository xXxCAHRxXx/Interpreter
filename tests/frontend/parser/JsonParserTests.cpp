#include <memory>
#include <sstream>

#include <gtest/gtest.h>
#include <nlohmann/json.hpp>

#include "TestSupport.hpp"
#include "error/SyntaxError.hpp"
#include "frontend/ast/expression/BinOp.hpp"
#include "frontend/ast/expression/Const.hpp"
#include "frontend/ast/expression/Var.hpp"
#include "frontend/ast/statement/Assign.hpp"
#include "frontend/ast/statement/Block.hpp"
#include "frontend/ast/statement/DoWhile.hpp"
#include "frontend/ast/statement/If.hpp"
#include "frontend/ast/statement/Read.hpp"
#include "frontend/ast/statement/Skip.hpp"
#include "frontend/ast/statement/While.hpp"
#include "frontend/ast/statement/Write.hpp"

TEST(JsonParserTest, Parse_SkipString_ReturnsSkip) {
    auto parser = CreateTestParser();
    std::istringstream program(R"("skip")");

    IStmtPtr statement = parser->Parse(program);

    EXPECT_NE(std::dynamic_pointer_cast<Skip>(statement.get()), nullptr);
}

TEST(JsonParserTest, Parse_InvalidJson_ThrowsSyntaxError) {
    auto parser = CreateTestParser();
    std::istringstream program("{not-json}");

    EXPECT_THROW(parser->Parse(program), SyntaxError);
}

TEST(JsonParserTest, Parse_EmptyInput_ThrowsSyntaxError) {
    auto parser = CreateTestParser();
    std::istringstream program("");

    EXPECT_THROW(parser->Parse(program), SyntaxError);
}

TEST(JsonParserTest, Parse_UnknownStatement_ThrowsSyntaxError) {
    auto parser = CreateTestParser();
    std::istringstream program(R"({"unknown": 1})");

    EXPECT_THROW(parser->Parse(program), SyntaxError);
}

TEST(JsonParserTest, ParseStatement_Assign_ReturnsAssignWithNameAndValue) {
    auto parser = CreateTestParser();
    const nlohmann::json node = {{"assn", {{"dst", "x"}, {"src", {{"const", 5}}}}}};

    IStmtPtr statement = parser->ParseStatement(node);

    auto assign = std::dynamic_pointer_cast<Assign>(statement.get());
    ASSERT_NE(assign, nullptr);
    EXPECT_EQ(assign->GetName(), "x");
    EXPECT_NE(std::dynamic_pointer_cast<Const>(assign->GetValue().get()), nullptr);
}

TEST(JsonParserTest, ParseStatement_AssignWithNumericName_ThrowsSyntaxError) {
    auto parser = CreateTestParser();
    const nlohmann::json node = {{"assn", {{"dst", 1}, {"src", {{"const", 5}}}}}};

    EXPECT_THROW(parser->ParseStatement(node), SyntaxError);
}

TEST(JsonParserTest, ParseStatement_AssignWithoutSource_ThrowsSyntaxError) {
    auto parser = CreateTestParser();
    const nlohmann::json node = {{"assn", {{"dst", "x"}}}};

    EXPECT_THROW(parser->ParseStatement(node), SyntaxError);
}

TEST(JsonParserTest, ParseStatement_Read_ReturnsReadWithName) {
    auto parser = CreateTestParser();
    const nlohmann::json node = {{"read", "n"}};

    IStmtPtr statement = parser->ParseStatement(node);

    auto read = std::dynamic_pointer_cast<Read>(statement.get());
    ASSERT_NE(read, nullptr);
    EXPECT_EQ(read->GetName(), "n");
}

TEST(JsonParserTest, ParseStatement_ReadWithObjectName_ThrowsSyntaxError) {
    auto parser = CreateTestParser();
    const nlohmann::json node = {{"read", {{"var", "n"}}}};

    EXPECT_THROW(parser->ParseStatement(node), SyntaxError);
}

TEST(JsonParserTest, ParseStatement_Write_ReturnsWriteWithExpression) {
    auto parser = CreateTestParser();
    const nlohmann::json node = {{"write", {{"var", "x"}}}};

    IStmtPtr statement = parser->ParseStatement(node);

    auto write = std::dynamic_pointer_cast<Write>(statement.get());
    ASSERT_NE(write, nullptr);
    EXPECT_NE(std::dynamic_pointer_cast<Var>(write->GetExpr().get()), nullptr);
}

TEST(JsonParserTest, ParseStatement_If_ReturnsIfWithBothBranches) {
    auto parser = CreateTestParser();
    const nlohmann::json node = {
        {"if", {{"cond", {{"var", "x"}}}, {"then", {{"write", {{"const", 1}}}}}, {"else", "skip"}}}
    };

    IStmtPtr statement = parser->ParseStatement(node);

    auto branch = std::dynamic_pointer_cast<If>(statement.get());
    ASSERT_NE(branch, nullptr);
    EXPECT_NE(std::dynamic_pointer_cast<Var>(branch->GetCond().get()), nullptr);
    EXPECT_NE(std::dynamic_pointer_cast<Write>(branch->GetThenBranch().get()), nullptr);
    EXPECT_NE(std::dynamic_pointer_cast<Skip>(branch->GetElseBranch().get()), nullptr);
}

TEST(JsonParserTest, ParseStatement_IfWithoutElse_ThrowsSyntaxError) {
    auto parser = CreateTestParser();
    const nlohmann::json node = {{"if", {{"cond", {{"var", "x"}}}, {"then", "skip"}}}};

    EXPECT_THROW(parser->ParseStatement(node), SyntaxError);
}

TEST(JsonParserTest, ParseStatement_While_ReturnsWhileWithBody) {
    auto parser = CreateTestParser();
    const nlohmann::json node = {{"while", {{"cond", {{"var", "x"}}}, {"body", "skip"}}}};

    IStmtPtr statement = parser->ParseStatement(node);

    auto loop = std::dynamic_pointer_cast<While>(statement.get());
    ASSERT_NE(loop, nullptr);
    EXPECT_NE(std::dynamic_pointer_cast<Var>(loop->GetCond().get()), nullptr);
    EXPECT_NE(std::dynamic_pointer_cast<Skip>(loop->GetBody().get()), nullptr);
}

TEST(JsonParserTest, ParseStatement_DoWhile_ReturnsDoWhileWithBody) {
    auto parser = CreateTestParser();
    const nlohmann::json node = {{"do", {{"body", "skip"}, {"cond", {{"var", "x"}}}}}};

    IStmtPtr statement = parser->ParseStatement(node);

    auto loop = std::dynamic_pointer_cast<DoWhile>(statement.get());
    ASSERT_NE(loop, nullptr);
    EXPECT_NE(std::dynamic_pointer_cast<Skip>(loop->GetBody().get()), nullptr);
    EXPECT_NE(std::dynamic_pointer_cast<Var>(loop->GetCond().get()), nullptr);
}

TEST(JsonParserTest, ParseStatement_NestedSequences_FlattensIntoBlock) {
    auto parser = CreateTestParser();
    const nlohmann::json node = {
        {"seq", {
            {"left", {{"assn", {{"dst", "x"}, {"src", {{"const", 1}}}}}}},
            {"right", {{"seq", {{"left", {{"write", {{"var", "x"}}}}}, {"right", "skip"}}}}}
        }}
    };

    IStmtPtr statement = parser->ParseStatement(node);

    auto block = std::dynamic_pointer_cast<Block>(statement.get());
    ASSERT_NE(block, nullptr);
    ASSERT_EQ(block->GetStatements().size(), 3U);
    EXPECT_NE(std::dynamic_pointer_cast<Assign>(block->GetStatements()[0].get()), nullptr);
    EXPECT_NE(std::dynamic_pointer_cast<Write>(block->GetStatements()[1].get()), nullptr);
    EXPECT_NE(std::dynamic_pointer_cast<Skip>(block->GetStatements()[2].get()), nullptr);
}

TEST(JsonParserTest, ParseStatement_SequenceWithoutRight_ThrowsSyntaxError) {
    auto parser = CreateTestParser();
    const nlohmann::json node = {{"seq", {{"left", "skip"}}}};

    EXPECT_THROW(parser->ParseStatement(node), SyntaxError);
}

TEST(JsonParserTest, ParseStatement_EmptyObject_ThrowsSyntaxError) {
    auto parser = CreateTestParser();
    const nlohmann::json node = nlohmann::json::object();

    EXPECT_THROW(parser->ParseStatement(node), SyntaxError);
}

TEST(JsonParserTest, ParseStatement_UnknownString_ThrowsSyntaxError) {
    auto parser = CreateTestParser();
    const nlohmann::json node = "jump";

    EXPECT_THROW(parser->ParseStatement(node), SyntaxError);
}

TEST(JsonParserTest, ParseExpression_Const_ReturnsConstWithValue) {
    auto parser = CreateTestParser();
    const nlohmann::json node = {{"const", 5}};

    IExprPtr expression = parser->ParseExpression(node);

    auto constant = std::dynamic_pointer_cast<Const>(expression.get());
    ASSERT_NE(constant, nullptr);
    EXPECT_EQ(constant->GetValue(), Value(5));
}

TEST(JsonParserTest, ParseExpression_NegativeConst_ReturnsNegativeValue) {
    auto parser = CreateTestParser();
    const nlohmann::json node = {{"const", -5}};

    IExprPtr expression = parser->ParseExpression(node);

    auto constant = std::dynamic_pointer_cast<Const>(expression.get());
    ASSERT_NE(constant, nullptr);
    EXPECT_EQ(constant->GetValue(), Value(-5));
}

TEST(JsonParserTest, ParseExpression_FractionalConst_ThrowsSyntaxError) {
    auto parser = CreateTestParser();
    const nlohmann::json node = {{"const", 1.5}};

    EXPECT_THROW(parser->ParseExpression(node), SyntaxError);
}

TEST(JsonParserTest, ParseExpression_StringConst_ThrowsSyntaxError) {
    auto parser = CreateTestParser();
    const nlohmann::json node = {{"const", "5"}};

    EXPECT_THROW(parser->ParseExpression(node), SyntaxError);
}

TEST(JsonParserTest, ParseExpression_Var_ReturnsVarWithName) {
    auto parser = CreateTestParser();
    const nlohmann::json node = {{"var", "x"}};

    IExprPtr expression = parser->ParseExpression(node);

    auto variable = std::dynamic_pointer_cast<Var>(expression.get());
    ASSERT_NE(variable, nullptr);
    EXPECT_EQ(variable->GetName(), "x");
}

TEST(JsonParserTest, ParseExpression_VarWithNumericName_ThrowsSyntaxError) {
    auto parser = CreateTestParser();
    const nlohmann::json node = {{"var", 1}};

    EXPECT_THROW(parser->ParseExpression(node), SyntaxError);
}

TEST(JsonParserTest, ParseExpression_BinOp_ReturnsBinOpWithOperatorAndOperands) {
    auto parser = CreateTestParser();
    const nlohmann::json node = {{"binop", "-"}, {"left", {{"const", 7}}}, {"right", {{"var", "x"}}}};

    IExprPtr expression = parser->ParseExpression(node);

    auto binary = std::dynamic_pointer_cast<BinOp>(expression.get());
    ASSERT_NE(binary, nullptr);
    EXPECT_EQ(binary->GetOperator()->Apply(Value(7), Value(3)), Value(4));
    EXPECT_NE(std::dynamic_pointer_cast<Const>(binary->GetLhs().get()), nullptr);
    EXPECT_NE(std::dynamic_pointer_cast<Var>(binary->GetRhs().get()), nullptr);
}

TEST(JsonParserTest, ParseExpression_UnknownOperator_ThrowsSyntaxError) {
    auto parser = CreateTestParser();
    const nlohmann::json node = {{"binop", "^"}, {"left", {{"const", 2}}}, {"right", {{"const", 3}}}};

    EXPECT_THROW(parser->ParseExpression(node), SyntaxError);
}

TEST(JsonParserTest, ParseExpression_BinOpWithoutRight_ThrowsSyntaxError) {
    auto parser = CreateTestParser();
    const nlohmann::json node = {{"binop", "+"}, {"left", {{"const", 2}}}};

    EXPECT_THROW(parser->ParseExpression(node), SyntaxError);
}

TEST(JsonParserTest, ParseExpression_BareString_ThrowsSyntaxError) {
    auto parser = CreateTestParser();
    const nlohmann::json node = "x";

    EXPECT_THROW(parser->ParseExpression(node), SyntaxError);
}

TEST(JsonParserTest, ParseExpression_UnknownNode_ThrowsSyntaxError) {
    auto parser = CreateTestParser();
    const nlohmann::json node = {{"unknown", 1}};

    EXPECT_THROW(parser->ParseExpression(node), SyntaxError);
}
