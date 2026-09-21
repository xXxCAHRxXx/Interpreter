#pragma once

#include "RuntimeError.hpp"

class DivisionByZero final : public RuntimeError {
public:
    DivisionByZero()
    : RuntimeError("division by zero")
    { }
};
