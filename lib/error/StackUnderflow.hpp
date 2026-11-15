#pragma once

#include "error/RuntimeError.hpp"

class StackUnderflow final : public RuntimeError {
public:
    StackUnderflow()
    : RuntimeError("stack underflow") { }
};
