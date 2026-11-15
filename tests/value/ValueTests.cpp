#include <sstream>

#include <gtest/gtest.h>

#include "error/DivisionByZero.hpp"
#include "value/Value.hpp"

TEST(ValueTest, Constructor_NoArguments_IsZero) {
    Value value;

    EXPECT_EQ(value, Value(0));
}

TEST(ValueTest, Constructor_TrueBoolean_IsOne) {
    Value value(true);

    EXPECT_EQ(value, Value(1));
}

TEST(ValueTest, Constructor_FalseBoolean_IsZero) {
    Value value(false);

    EXPECT_EQ(value, Value(0));
}

TEST(ValueTest, OperatorBool_Zero_ReturnsFalse) {
    const Value value(0);

    bool result = static_cast<bool>(value);

    EXPECT_FALSE(result);
}

TEST(ValueTest, OperatorBool_Negative_ReturnsTrue) {
    const Value value(-1);

    bool result = static_cast<bool>(value);

    EXPECT_TRUE(result);
}

TEST(ValueTest, OperatorBool_Positive_ReturnsTrue) {
    const Value value(42);

    bool result = static_cast<bool>(value);

    EXPECT_TRUE(result);
}

TEST(ValueTest, OperatorPlus_TwoPositives_ReturnsSum) {
    const Value lhs(7);
    const Value rhs(3);

    Value result = lhs + rhs;

    EXPECT_EQ(result, Value(10));
}

TEST(ValueTest, OperatorPlus_Zero_ReturnsSameValue) {
    const Value lhs(7);
    const Value rhs(0);

    Value result = lhs + rhs;

    EXPECT_EQ(result, Value(7));
}

TEST(ValueTest, OperatorPlus_OppositeValues_ReturnsZero) {
    const Value lhs(5);
    const Value rhs(-5);

    Value result = lhs + rhs;

    EXPECT_EQ(result, Value(0));
}

TEST(ValueTest, OperatorMinus_LargerLhs_ReturnsPositiveDifference) {
    const Value lhs(7);
    const Value rhs(3);

    Value result = lhs - rhs;

    EXPECT_EQ(result, Value(4));
}

TEST(ValueTest, OperatorMinus_SmallerLhs_ReturnsNegativeDifference) {
    const Value lhs(3);
    const Value rhs(7);

    Value result = lhs - rhs;

    EXPECT_EQ(result, Value(-4));
}

TEST(ValueTest, OperatorMinus_EqualValues_ReturnsZero) {
    const Value lhs(7);
    const Value rhs(7);

    Value result = lhs - rhs;

    EXPECT_EQ(result, Value(0));
}

TEST(ValueTest, OperatorMultiply_TwoPositives_ReturnsProduct) {
    const Value lhs(7);
    const Value rhs(3);

    Value result = lhs * rhs;

    EXPECT_EQ(result, Value(21));
}

TEST(ValueTest, OperatorMultiply_Zero_ReturnsZero) {
    const Value lhs(7);
    const Value rhs(0);

    Value result = lhs * rhs;

    EXPECT_EQ(result, Value(0));
}

TEST(ValueTest, OperatorMultiply_NegativeAndPositive_ReturnsNegative) {
    const Value lhs(-7);
    const Value rhs(3);

    Value result = lhs * rhs;

    EXPECT_EQ(result, Value(-21));
}

TEST(ValueTest, OperatorDivide_ZeroDivisor_ThrowsDivisionByZero) {
    const Value lhs(7);
    const Value rhs(0);

    EXPECT_THROW(lhs / rhs, DivisionByZero);
}

TEST(ValueTest, OperatorDivide_NotDivisible_TruncatesResult) {
    const Value lhs(7);
    const Value rhs(2);

    Value result = lhs / rhs;

    EXPECT_EQ(result, Value(3));
}

TEST(ValueTest, OperatorDivide_NegativeDividend_TruncatesTowardZero) {
    const Value lhs(-7);
    const Value rhs(2);

    Value result = lhs / rhs;

    EXPECT_EQ(result, Value(-3));
}

TEST(ValueTest, OperatorDivide_DividendLessThanDivisor_ReturnsZero) {
    const Value lhs(2);
    const Value rhs(7);

    Value result = lhs / rhs;

    EXPECT_EQ(result, Value(0));
}

TEST(ValueTest, OperatorModulo_ZeroDivisor_ThrowsDivisionByZero) {
    const Value lhs(7);
    const Value rhs(0);

    EXPECT_THROW(lhs % rhs, DivisionByZero);
}

TEST(ValueTest, OperatorModulo_NotDivisible_ReturnsRemainder) {
    const Value lhs(7);
    const Value rhs(3);

    Value result = lhs % rhs;

    EXPECT_EQ(result, Value(1));
}

TEST(ValueTest, OperatorModulo_NegativeDividend_ReturnsNegativeRemainder) {
    const Value lhs(-7);
    const Value rhs(3);

    Value result = lhs % rhs;

    EXPECT_EQ(result, Value(-1));
}

TEST(ValueTest, OperatorModulo_Divisible_ReturnsZero) {
    const Value lhs(9);
    const Value rhs(3);

    Value result = lhs % rhs;

    EXPECT_EQ(result, Value(0));
}

TEST(ValueTest, OperatorEqual_SameValues_ReturnsTrue) {
    const Value lhs(5);
    const Value rhs(5);

    bool result = lhs == rhs;

    EXPECT_TRUE(result);
}

TEST(ValueTest, OperatorEqual_DifferentValues_ReturnsFalse) {
    const Value lhs(5);
    const Value rhs(6);

    bool result = lhs == rhs;

    EXPECT_FALSE(result);
}

TEST(ValueTest, OperatorEqual_OppositeValues_ReturnsFalse) {
    const Value lhs(5);
    const Value rhs(-5);

    bool result = lhs == rhs;

    EXPECT_FALSE(result);
}

TEST(ValueTest, OperatorNotEqual_DifferentValues_ReturnsTrue) {
    const Value lhs(5);
    const Value rhs(6);

    bool result = lhs != rhs;

    EXPECT_TRUE(result);
}

TEST(ValueTest, OperatorNotEqual_SameValues_ReturnsFalse) {
    const Value lhs(5);
    const Value rhs(5);

    bool result = lhs != rhs;

    EXPECT_FALSE(result);
}

TEST(ValueTest, OperatorLess_SmallerLhs_ReturnsTrue) {
    const Value lhs(2);
    const Value rhs(3);

    bool result = lhs < rhs;

    EXPECT_TRUE(result);
}

TEST(ValueTest, OperatorLess_EqualValues_ReturnsFalse) {
    const Value lhs(3);
    const Value rhs(3);

    bool result = lhs < rhs;

    EXPECT_FALSE(result);
}

TEST(ValueTest, OperatorLess_NegativeLhs_ReturnsTrue) {
    const Value lhs(-3);
    const Value rhs(2);

    bool result = lhs < rhs;

    EXPECT_TRUE(result);
}

TEST(ValueTest, OperatorGreater_LargerLhs_ReturnsTrue) {
    const Value lhs(3);
    const Value rhs(2);

    bool result = lhs > rhs;

    EXPECT_TRUE(result);
}

TEST(ValueTest, OperatorGreater_EqualValues_ReturnsFalse) {
    const Value lhs(3);
    const Value rhs(3);

    bool result = lhs > rhs;

    EXPECT_FALSE(result);
}

TEST(ValueTest, OperatorGreater_SmallerLhs_ReturnsFalse) {
    const Value lhs(2);
    const Value rhs(3);

    bool result = lhs > rhs;

    EXPECT_FALSE(result);
}

TEST(ValueTest, OperatorLessEqual_SmallerLhs_ReturnsTrue) {
    const Value lhs(2);
    const Value rhs(3);

    bool result = lhs <= rhs;

    EXPECT_TRUE(result);
}

TEST(ValueTest, OperatorLessEqual_EqualValues_ReturnsTrue) {
    const Value lhs(3);
    const Value rhs(3);

    bool result = lhs <= rhs;

    EXPECT_TRUE(result);
}

TEST(ValueTest, OperatorLessEqual_LargerLhs_ReturnsFalse) {
    const Value lhs(4);
    const Value rhs(3);

    bool result = lhs <= rhs;

    EXPECT_FALSE(result);
}

TEST(ValueTest, OperatorGreaterEqual_LargerLhs_ReturnsTrue) {
    const Value lhs(4);
    const Value rhs(3);

    bool result = lhs >= rhs;

    EXPECT_TRUE(result);
}

TEST(ValueTest, OperatorGreaterEqual_EqualValues_ReturnsTrue) {
    const Value lhs(3);
    const Value rhs(3);

    bool result = lhs >= rhs;

    EXPECT_TRUE(result);
}

TEST(ValueTest, OperatorGreaterEqual_SmallerLhs_ReturnsFalse) {
    const Value lhs(2);
    const Value rhs(3);

    bool result = lhs >= rhs;

    EXPECT_FALSE(result);
}

TEST(ValueTest, OperatorOutput_Positive_WritesNumber) {
    std::ostringstream output;

    output << Value(42);

    EXPECT_EQ(output.str(), "42");
}

TEST(ValueTest, OperatorOutput_Negative_WritesMinusSign) {
    std::ostringstream output;

    output << Value(-42);

    EXPECT_EQ(output.str(), "-42");
}

TEST(ValueTest, OperatorOutput_TrueBoolean_WritesOne) {
    std::ostringstream output;

    output << Value(true);

    EXPECT_EQ(output.str(), "1");
}

TEST(ValueTest, OperatorInput_Number_ReadsValue) {
    std::istringstream input("42");
    Value value;

    input >> value;

    EXPECT_FALSE(input.fail());
    EXPECT_EQ(value, Value(42));
}

TEST(ValueTest, OperatorInput_NotANumber_SetsFailState) {
    std::istringstream input("abc");
    Value value;

    input >> value;

    EXPECT_TRUE(input.fail());
}

TEST(ValueTest, OperatorInput_NegativeWithLeadingSpaces_ReadsValue) {
    std::istringstream input("   -17");
    Value value;

    input >> value;

    EXPECT_FALSE(input.fail());
    EXPECT_EQ(value, Value(-17));
}

TEST(ValueTest, OperatorInput_OutOfRangeNumber_SetsFailState) {
    std::istringstream input("2147483648");
    Value value;

    input >> value;

    EXPECT_TRUE(input.fail());
}
