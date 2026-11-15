#include <string>

#include <gtest/gtest.h>
#include <nlohmann/json.hpp>

#include "error/SyntaxError.hpp"
#include "frontend/parser/json/JsonUtils.hpp"

TEST(JsonUtilsTest, GetField_ExistingKey_ReturnsField) {
    const nlohmann::json node = {{"dst", "x"}, {"src", 5}};

    const nlohmann::json& field = GetField(node, "src");

    EXPECT_EQ(field, nlohmann::json(5));
}

TEST(JsonUtilsTest, GetField_MissingKey_ThrowsSyntaxError) {
    const nlohmann::json node = {{"dst", "x"}};

    EXPECT_THROW(GetField(node, "src"), SyntaxError);
}

TEST(JsonUtilsTest, GetField_NotAnObject_ThrowsSyntaxError) {
    const nlohmann::json node = nlohmann::json::array({1, 2, 3});

    EXPECT_THROW(GetField(node, "src"), SyntaxError);
}

TEST(JsonUtilsTest, GetField_NullValue_ReturnsNullField) {
    const nlohmann::json node = {{"src", nullptr}};

    const nlohmann::json& field = GetField(node, "src");

    EXPECT_TRUE(field.is_null());
}

TEST(JsonUtilsTest, GetName_String_ReturnsName) {
    const nlohmann::json node = "counter";

    std::string name = GetName(node);

    EXPECT_EQ(name, "counter");
}

TEST(JsonUtilsTest, GetName_Number_ThrowsSyntaxError) {
    const nlohmann::json node = 42;

    EXPECT_THROW(GetName(node), SyntaxError);
}

TEST(JsonUtilsTest, GetName_EmptyString_ReturnsEmptyName) {
    const nlohmann::json node = "";

    std::string name = GetName(node);

    EXPECT_EQ(name, "");
}
