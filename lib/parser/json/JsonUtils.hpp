#pragma once

#include <string>

#include <nlohmann/json.hpp>

const nlohmann::json& GetField(const nlohmann::json& node, const char* key);

std::string GetName(const nlohmann::json& node);