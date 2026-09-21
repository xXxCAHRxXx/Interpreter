#pragma once

#include "error/RuntimeError.hpp"

class InvalidInput final : public RuntimeError {
public:
    InvalidInput()
    : RuntimeError("read: invalid or missing input")
    { }
};