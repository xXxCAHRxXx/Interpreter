#pragma once

#include <string>

#include "error/RuntimeError.hpp"

class UndefinedVariable final : public RuntimeError {
public:
    explicit UndefinedVariable(const std::string& name)
    : RuntimeError("undefined variable: " + name) 
    { }
};