#include <sstream>

#include <gtest/gtest.h>

#include "backend/runtime/io/StreamOutput.hpp"

TEST(StreamOutputTest, Write_Value_WritesValueWithNewline) {
    std::ostringstream stream;
    StreamOutput output(stream);

    output.Write(Value(42));

    EXPECT_EQ(stream.str(), "42\n");
}

TEST(StreamOutputTest, Write_SeveralValues_WritesEachOnOwnLine) {
    std::ostringstream stream;
    StreamOutput output(stream);

    output.Write(Value(1));
    output.Write(Value(2));
    output.Write(Value(3));

    EXPECT_EQ(stream.str(), "1\n2\n3\n");
}

TEST(StreamOutputTest, Write_NegativeValue_WritesMinusSign) {
    std::ostringstream stream;
    StreamOutput output(stream);

    output.Write(Value(-7));

    EXPECT_EQ(stream.str(), "-7\n");
}

TEST(StreamOutputTest, Write_ZeroValue_WritesZero) {
    std::ostringstream stream;
    StreamOutput output(stream);

    output.Write(Value(0));

    EXPECT_EQ(stream.str(), "0\n");
}
