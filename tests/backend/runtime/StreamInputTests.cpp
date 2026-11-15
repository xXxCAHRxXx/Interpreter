#include <sstream>

#include <gtest/gtest.h>

#include "backend/runtime/io/StreamInput.hpp"
#include "error/InvalidInput.hpp"

TEST(StreamInputTest, Read_Number_ReturnsValue) {
    std::istringstream stream("42");
    StreamInput input(stream);

    Value value = input.Read();

    EXPECT_EQ(value, Value(42));
}

TEST(StreamInputTest, Read_NotANumber_ThrowsInvalidInput) {
    std::istringstream stream("abc");
    StreamInput input(stream);

    EXPECT_THROW(input.Read(), InvalidInput);
}

TEST(StreamInputTest, Read_EmptyStream_ThrowsInvalidInput) {
    std::istringstream stream("");
    StreamInput input(stream);

    EXPECT_THROW(input.Read(), InvalidInput);
}

TEST(StreamInputTest, Read_OutOfRangeNumber_ThrowsInvalidInput) {
    std::istringstream stream("2147483648");
    StreamInput input(stream);

    EXPECT_THROW(input.Read(), InvalidInput);
}

TEST(StreamInputTest, Read_NegativeNumber_ReturnsNegativeValue) {
    std::istringstream stream("-17");
    StreamInput input(stream);

    Value value = input.Read();

    EXPECT_EQ(value, Value(-17));
}

TEST(StreamInputTest, Read_SeveralNumbers_ReturnsThemInOrder) {
    std::istringstream stream("1\n2 3");
    StreamInput input(stream);

    Value first = input.Read();
    Value second = input.Read();
    Value third = input.Read();

    EXPECT_EQ(first, Value(1));
    EXPECT_EQ(second, Value(2));
    EXPECT_EQ(third, Value(3));
}

TEST(StreamInputTest, Read_AfterLastNumber_ThrowsInvalidInput) {
    std::istringstream stream("5");
    StreamInput input(stream);
    input.Read();

    EXPECT_THROW(input.Read(), InvalidInput);
}
