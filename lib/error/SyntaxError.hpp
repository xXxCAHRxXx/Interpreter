#pragma once

#include "InterpreterError.hpp"

class SyntaxError : public InterpreterError {
public:
    using InterpreterError::InterpreterError;
};