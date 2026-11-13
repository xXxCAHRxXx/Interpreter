#pragma once

#include <unordered_map>
#include <string>
#include <string_view>

#include "value/Value.hpp"
#include "IValueStore.hpp"

class Environment final : public IValueStore {
public:
    Value Get(std::string_view name) const override;

    void Set(std::string_view name, const Value& value) override;

private:
    std::unordered_map<std::string, Value> variables_;
};
