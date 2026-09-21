#include <sstream>

#include <gtest/gtest.h>

#include "error/DivisionByZero.hpp"
#include "value/Value.hpp"

TEST(ValueTest, PerformsArithmeticOperations) {
    const Value seven(7);
    const Value three(3);

    EXPECT_EQ(seven + three, Value(10));
    EXPECT_EQ(seven - three, Value(4));
    EXPECT_EQ(seven * three, Value(21));
    EXPECT_EQ(seven / three, Value(2));
    EXPECT_EQ(seven % three, Value(1));
}

TEST(ValueTest, ComparesValues) {
    const Value two(2);
    const Value three(3);

    EXPECT_TRUE(two < three);
    EXPECT_TRUE(two <= three);
    EXPECT_TRUE(three > two);
    EXPECT_TRUE(three >= two);
    EXPECT_TRUE(two == Value(2));
    EXPECT_TRUE(two != three);
}

TEST(ValueTest, ConvertsZeroAndNonZeroToBoolean) {
    EXPECT_FALSE(static_cast<bool>(Value(0)));
    EXPECT_TRUE(static_cast<bool>(Value(1)));
    EXPECT_TRUE(static_cast<bool>(Value(-1)));
}

TEST(ValueTest, RejectsDivisionAndModuloByZero) {
    EXPECT_THROW(Value(10) / Value(0), DivisionByZero);
    EXPECT_THROW(Value(10) % Value(0), DivisionByZero);
}

TEST(ValueTest, ReadsAndWritesStreams) {
    std::istringstream input("42");
    std::ostringstream output;
    Value value;

    input >> value;
    output << value;

    EXPECT_EQ(value, Value(42));
    EXPECT_EQ(output.str(), "42");
}
