#include <gtest/gtest.h>

#include "binary_operator/arithmetic/Add.hpp"
#include "binary_operator/arithmetic/Div.hpp"
#include "binary_operator/arithmetic/Mod.hpp"
#include "binary_operator/arithmetic/Mul.hpp"
#include "binary_operator/arithmetic/Sub.hpp"
#include "binary_operator/combinator/Swapped.hpp"
#include "binary_operator/comparison/Eq.hpp"
#include "binary_operator/comparison/Ge.hpp"
#include "binary_operator/comparison/Gt.hpp"
#include "binary_operator/comparison/Le.hpp"
#include "binary_operator/comparison/Lt.hpp"
#include "binary_operator/comparison/Ne.hpp"
#include "binary_operator/logical/And.hpp"
#include "binary_operator/logical/Negated.hpp"
#include "binary_operator/logical/Or.hpp"
#include "error/DivisionByZero.hpp"

TEST(AddTest, Apply_TwoPositives_ReturnsSum) {
    const Add add;

    Value result = add.Apply(Value(7), Value(3));

    EXPECT_EQ(result, Value(10));
}

TEST(AddTest, Apply_NegativeRhs_ReturnsDifference) {
    const Add add;

    Value result = add.Apply(Value(7), Value(-3));

    EXPECT_EQ(result, Value(4));
}

TEST(SubTest, Apply_LargerLhs_ReturnsPositiveDifference) {
    const Sub sub;

    Value result = sub.Apply(Value(7), Value(3));

    EXPECT_EQ(result, Value(4));
}

TEST(SubTest, Apply_SmallerLhs_ReturnsNegativeDifference) {
    const Sub sub;

    Value result = sub.Apply(Value(3), Value(7));

    EXPECT_EQ(result, Value(-4));
}

TEST(MulTest, Apply_TwoPositives_ReturnsProduct) {
    const Mul mul;

    Value result = mul.Apply(Value(7), Value(3));

    EXPECT_EQ(result, Value(21));
}

TEST(MulTest, Apply_ZeroOperand_ReturnsZero) {
    const Mul mul;

    Value result = mul.Apply(Value(0), Value(3));

    EXPECT_EQ(result, Value(0));
}

TEST(DivTest, Apply_ZeroDivisor_ThrowsDivisionByZero) {
    const Div div;

    EXPECT_THROW(div.Apply(Value(7), Value(0)), DivisionByZero);
}

TEST(DivTest, Apply_NotDivisible_TruncatesResult) {
    const Div div;

    Value result = div.Apply(Value(7), Value(2));

    EXPECT_EQ(result, Value(3));
}

TEST(DivTest, Apply_NegativeDividend_TruncatesTowardZero) {
    const Div div;

    Value result = div.Apply(Value(-7), Value(2));

    EXPECT_EQ(result, Value(-3));
}

TEST(ModTest, Apply_ZeroDivisor_ThrowsDivisionByZero) {
    const Mod mod;

    EXPECT_THROW(mod.Apply(Value(7), Value(0)), DivisionByZero);
}

TEST(ModTest, Apply_NotDivisible_ReturnsRemainder) {
    const Mod mod;

    Value result = mod.Apply(Value(7), Value(3));

    EXPECT_EQ(result, Value(1));
}

TEST(ModTest, Apply_NegativeDividend_ReturnsNegativeRemainder) {
    const Mod mod;

    Value result = mod.Apply(Value(-7), Value(3));

    EXPECT_EQ(result, Value(-1));
}

TEST(EqTest, Apply_EqualValues_ReturnsOne) {
    const Eq eq;

    Value result = eq.Apply(Value(5), Value(5));

    EXPECT_EQ(result, Value(1));
}

TEST(EqTest, Apply_DifferentValues_ReturnsZero) {
    const Eq eq;

    Value result = eq.Apply(Value(5), Value(6));

    EXPECT_EQ(result, Value(0));
}

TEST(NeTest, Apply_EqualValues_ReturnsZero) {
    const Ne ne;

    Value result = ne.Apply(Value(5), Value(5));

    EXPECT_EQ(result, Value(0));
}

TEST(NeTest, Apply_DifferentValues_ReturnsOne) {
    const Ne ne;

    Value result = ne.Apply(Value(5), Value(6));

    EXPECT_EQ(result, Value(1));
}

TEST(GtTest, Apply_LargerLhs_ReturnsOne) {
    const Gt gt;

    Value result = gt.Apply(Value(3), Value(2));

    EXPECT_EQ(result, Value(1));
}

TEST(GtTest, Apply_EqualValues_ReturnsZero) {
    const Gt gt;

    Value result = gt.Apply(Value(3), Value(3));

    EXPECT_EQ(result, Value(0));
}

TEST(GtTest, Apply_SmallerLhs_ReturnsZero) {
    const Gt gt;

    Value result = gt.Apply(Value(2), Value(3));

    EXPECT_EQ(result, Value(0));
}

TEST(LtTest, Apply_SmallerLhs_ReturnsOne) {
    const Lt lt;

    Value result = lt.Apply(Value(2), Value(3));

    EXPECT_EQ(result, Value(1));
}

TEST(LtTest, Apply_EqualValues_ReturnsZero) {
    const Lt lt;

    Value result = lt.Apply(Value(3), Value(3));

    EXPECT_EQ(result, Value(0));
}

TEST(LtTest, Apply_LargerLhs_ReturnsZero) {
    const Lt lt;

    Value result = lt.Apply(Value(3), Value(2));

    EXPECT_EQ(result, Value(0));
}

TEST(LeTest, Apply_SmallerLhs_ReturnsOne) {
    const Le le;

    Value result = le.Apply(Value(2), Value(3));

    EXPECT_EQ(result, Value(1));
}

TEST(LeTest, Apply_EqualValues_ReturnsOne) {
    const Le le;

    Value result = le.Apply(Value(3), Value(3));

    EXPECT_EQ(result, Value(1));
}

TEST(LeTest, Apply_LargerLhs_ReturnsZero) {
    const Le le;

    Value result = le.Apply(Value(4), Value(3));

    EXPECT_EQ(result, Value(0));
}

TEST(GeTest, Apply_LargerLhs_ReturnsOne) {
    const Ge ge;

    Value result = ge.Apply(Value(4), Value(3));

    EXPECT_EQ(result, Value(1));
}

TEST(GeTest, Apply_EqualValues_ReturnsOne) {
    const Ge ge;

    Value result = ge.Apply(Value(3), Value(3));

    EXPECT_EQ(result, Value(1));
}

TEST(GeTest, Apply_SmallerLhs_ReturnsZero) {
    const Ge ge;

    Value result = ge.Apply(Value(2), Value(3));

    EXPECT_EQ(result, Value(0));
}

TEST(AndTest, Apply_BothNonZero_ReturnsOne) {
    const And conjunction;

    Value result = conjunction.Apply(Value(1), Value(1));

    EXPECT_EQ(result, Value(1));
}

TEST(AndTest, Apply_OneZero_ReturnsZero) {
    const And conjunction;

    Value result = conjunction.Apply(Value(1), Value(0));

    EXPECT_EQ(result, Value(0));
}

TEST(AndTest, Apply_NonOneValues_ReturnsOne) {
    const And conjunction;

    Value result = conjunction.Apply(Value(5), Value(-3));

    EXPECT_EQ(result, Value(1));
}

TEST(OrTest, Apply_BothZero_ReturnsZero) {
    const Or disjunction;

    Value result = disjunction.Apply(Value(0), Value(0));

    EXPECT_EQ(result, Value(0));
}

TEST(OrTest, Apply_OneNonZero_ReturnsOne) {
    const Or disjunction;

    Value result = disjunction.Apply(Value(0), Value(1));

    EXPECT_EQ(result, Value(1));
}

TEST(OrTest, Apply_NonOneValue_ReturnsOne) {
    const Or disjunction;

    Value result = disjunction.Apply(Value(0), Value(7));

    EXPECT_EQ(result, Value(1));
}

TEST(SwappedTest, Apply_NonCommutativeOperator_SwapsOperands) {
    const Swapped<Sub> swapped;

    Value result = swapped.Apply(Value(7), Value(3));

    EXPECT_EQ(result, Value(-4));
}

TEST(SwappedTest, Apply_ZeroLhsWithDivision_ThrowsDivisionByZero) {
    const Swapped<Div> swapped;

    EXPECT_THROW(swapped.Apply(Value(0), Value(5)), DivisionByZero);
}

TEST(SwappedTest, Apply_CommutativeOperator_ReturnsSameResult) {
    const Swapped<Add> swapped;

    Value result = swapped.Apply(Value(7), Value(3));

    EXPECT_EQ(result, Value(10));
}

TEST(NegatedTest, Apply_TrueInnerResult_ReturnsZero) {
    const Negated<Eq> negated;

    Value result = negated.Apply(Value(2), Value(2));

    EXPECT_EQ(result, Value(0));
}

TEST(NegatedTest, Apply_FalseInnerResult_ReturnsOne) {
    const Negated<Eq> negated;

    Value result = negated.Apply(Value(2), Value(3));

    EXPECT_EQ(result, Value(1));
}

TEST(NegatedTest, Apply_NonBooleanInnerResult_ReturnsZero) {
    const Negated<Sub> negated;

    Value result = negated.Apply(Value(5), Value(3));

    EXPECT_EQ(result, Value(0));
}
