#pragma once

#include "InterpreterError.hpp"

class RuntimeError : public InterpreterError {
public:
    using InterpreterError::InterpreterError;
};