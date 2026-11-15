#include <memory>

#include <gtest/gtest.h>

#include "binary_operator/arithmetic/Add.hpp"
#include "binary_operator/arithmetic/Sub.hpp"
#include "binary_operator/comparison/Le.hpp"
#include "binary_operator/registry/BinaryOperatorRegistry.hpp"
#include "binary_operator/registry/factory/BinaryOperatorRegistryFactory.hpp"
#include "error/LogicError.hpp"
#include "error/SyntaxError.hpp"

TEST(BinaryOperatorRegistryTest, Get_RegisteredSymbol_ReturnsOperator) {
    BinaryOperatorRegistry registry = BinaryOperatorRegistry::Builder()
        .Add("+", std::make_shared<Add>())
        .Add("-", std::make_shared<Sub>())
        .Build();

    IBinaryOperatorPtr op = registry.Get("-");

    EXPECT_EQ(op->Apply(Value(7), Value(3)), Value(4));
}

TEST(BinaryOperatorRegistryTest, Get_UnknownSymbol_ThrowsSyntaxError) {
    BinaryOperatorRegistry registry = BinaryOperatorRegistry::Builder()
        .Add("+", std::make_shared<Add>())
        .Build();

    EXPECT_THROW(registry.Get("^"), SyntaxError);
}

TEST(BinaryOperatorRegistryTest, Get_EmptyRegistry_ThrowsSyntaxError) {
    BinaryOperatorRegistry registry = BinaryOperatorRegistry::Builder().Build();

    EXPECT_THROW(registry.Get("+"), SyntaxError);
}

TEST(BinaryOperatorRegistryTest, Get_PrefixOfRegisteredSymbol_ThrowsSyntaxError) {
    BinaryOperatorRegistry registry = BinaryOperatorRegistry::Builder()
        .Add("<=", std::make_shared<Le>())
        .Build();

    EXPECT_THROW(registry.Get("<"), SyntaxError);
}

TEST(BinaryOperatorRegistryTest, Add_DuplicateSymbol_ThrowsLogicError) {
    BinaryOperatorRegistry::Builder builder;
    builder.Add("+", std::make_shared<Add>());

    EXPECT_THROW(builder.Add("+", std::make_shared<Sub>()), LogicError);
}

TEST(BinaryOperatorRegistryTest, Add_SameOperatorUnderTwoSymbols_BothSymbolsResolve) {
    auto add = std::make_shared<Add>();

    BinaryOperatorRegistry registry = BinaryOperatorRegistry::Builder()
        .Add("+", add)
        .Add("plus", add)
        .Build();

    EXPECT_EQ(registry.Get("+")->Apply(Value(2), Value(3)), Value(5));
    EXPECT_EQ(registry.Get("plus")->Apply(Value(2), Value(3)), Value(5));
}

TEST(BinaryOperatorRegistryFactoryTest, Create_ArithmeticSymbols_ResolveToArithmeticOperators) {
    const BinaryOperatorRegistryFactory factory;

    BinaryOperatorRegistry registry = factory.Create();

    EXPECT_EQ(registry.Get("+")->Apply(Value(7), Value(3)), Value(10));
    EXPECT_EQ(registry.Get("-")->Apply(Value(7), Value(3)), Value(4));
    EXPECT_EQ(registry.Get("*")->Apply(Value(7), Value(3)), Value(21));
    EXPECT_EQ(registry.Get("/")->Apply(Value(7), Value(3)), Value(2));
    EXPECT_EQ(registry.Get("%")->Apply(Value(7), Value(3)), Value(1));
}

TEST(BinaryOperatorRegistryFactoryTest, Create_ComparisonSymbols_ResolveToComparisonOperators) {
    const BinaryOperatorRegistryFactory factory;

    BinaryOperatorRegistry registry = factory.Create();

    EXPECT_EQ(registry.Get("<")->Apply(Value(3), Value(7)), Value(1));
    EXPECT_EQ(registry.Get("<=")->Apply(Value(7), Value(7)), Value(1));
    EXPECT_EQ(registry.Get(">")->Apply(Value(3), Value(7)), Value(0));
    EXPECT_EQ(registry.Get(">=")->Apply(Value(7), Value(7)), Value(1));
    EXPECT_EQ(registry.Get("==")->Apply(Value(7), Value(7)), Value(1));
    EXPECT_EQ(registry.Get("!=")->Apply(Value(7), Value(7)), Value(0));
}

TEST(BinaryOperatorRegistryFactoryTest, Create_LogicalSymbols_ResolveToLogicalOperators) {
    const BinaryOperatorRegistryFactory factory;

    BinaryOperatorRegistry registry = factory.Create();

    EXPECT_EQ(registry.Get("&&")->Apply(Value(1), Value(0)), Value(0));
    EXPECT_EQ(registry.Get("!!")->Apply(Value(1), Value(0)), Value(1));
}

TEST(BinaryOperatorRegistryFactoryTest, Create_CStyleOrSymbol_ThrowsSyntaxError) {
    const BinaryOperatorRegistryFactory factory;

    BinaryOperatorRegistry registry = factory.Create();

    EXPECT_THROW(registry.Get("||"), SyntaxError);
}
