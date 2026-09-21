#include "Environment.hpp"

#include "error/UndefinedVariable.hpp"

Value Environment::Get(std::string_view name) const {
    auto it = variables_.find(std::string(name));
    if (it == variables_.end()) {
        throw UndefinedVariable(std::string(name));
    }

    return it->second;
}

void Environment::Set(std::string_view name, const Value& value) {
    variables_.insert_or_assign(std::string(name), value);
}