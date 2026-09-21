#include <gtest/gtest.h>

#include "error/UndefinedVariable.hpp"
#include "runtime/environment/Environment.hpp"

TEST(EnvironmentTest, StoresAndUpdatesVariables) {
    Environment environment;

    environment.Set("answer", Value(41));
    EXPECT_EQ(environment.Get("answer"), Value(41));

    environment.Set("answer", Value(42));
    EXPECT_EQ(environment.Get("answer"), Value(42));
}

TEST(EnvironmentTest, RejectsUnknownVariables) {
    const Environment environment;

    EXPECT_THROW(environment.Get("missing"), UndefinedVariable);
}
