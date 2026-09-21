#pragma once

#include <gsl/pointers>

#include "value/Value.hpp"

class IInput {
public:
    IInput(const IInput&) = delete;
    IInput& operator=(const IInput&) = delete;

    virtual Value Read() = 0;

    virtual ~IInput() = default;

protected:
    IInput() = default;
};

using IInputPtr = gsl::not_null<std::shared_ptr<IInput>>;