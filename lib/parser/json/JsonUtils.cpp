#include "JsonUtils.hpp"

#include "error/SyntaxError.hpp"

const nlohmann::json& GetField(const nlohmann::json& node, const char* key) {
    if (!node.is_object() || !node.contains(key)) {
        throw SyntaxError(std::string("missing field: ") + key);
    }

    return node.at(key);
}

std::string GetName(const nlohmann::json& node) {
    if (!node.is_string()) {
        throw SyntaxError("name must be a string");
    }

    return node.get<std::string>();
}