#include <gtest/gtest.h>

#include "backend/runtime/environment/Environment.hpp"
#include "error/UndefinedVariable.hpp"

TEST(EnvironmentTest, Get_StoredVariable_ReturnsValue) {
    Environment environment;
    environment.Set("x", Value(41));

    Value value = environment.Get("x");

    EXPECT_EQ(value, Value(41));
}

TEST(EnvironmentTest, Get_UndefinedVariable_ThrowsUndefinedVariable) {
    const Environment environment;

    EXPECT_THROW(environment.Get("missing"), UndefinedVariable);
}

TEST(EnvironmentTest, Get_NameInDifferentCase_ThrowsUndefinedVariable) {
    Environment environment;
    environment.Set("x", Value(1));

    EXPECT_THROW(environment.Get("X"), UndefinedVariable);
}

TEST(EnvironmentTest, Set_ExistingVariable_OverwritesValue) {
    Environment environment;
    environment.Set("x", Value(41));

    environment.Set("x", Value(42));

    EXPECT_EQ(environment.Get("x"), Value(42));
}

TEST(EnvironmentTest, Set_TwoVariables_KeepsBothValues) {
    Environment environment;

    environment.Set("x", Value(1));
    environment.Set("y", Value(2));

    EXPECT_EQ(environment.Get("x"), Value(1));
    EXPECT_EQ(environment.Get("y"), Value(2));
}

TEST(EnvironmentTest, Set_EmptyName_StoresValue) {
    Environment environment;

    environment.Set("", Value(7));

    EXPECT_EQ(environment.Get(""), Value(7));
}
