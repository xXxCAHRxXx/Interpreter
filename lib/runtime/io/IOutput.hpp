#pragma once

#include <gsl/pointers>

#include "value/Value.hpp"

class IOutput {
public:
    IOutput(const IOutput&) = delete;
    IOutput& operator=(const IOutput&) = delete;

    virtual void Write(const Value& value) = 0;

    virtual ~IOutput() = default;

protected:
    IOutput() = default;
};

using IOutputPtr = gsl::not_null<std::shared_ptr<IOutput>>;