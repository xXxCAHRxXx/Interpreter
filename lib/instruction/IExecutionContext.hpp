#pragma once

#include <string_view>

#include "Label.hpp"
#include "value/Value.hpp"

class IExecutionContext {
public:
    IExecutionContext(const IExecutionContext&) = delete;
    IExecutionContext& operator=(const IExecutionContext&) = delete;

    virtual void PushToStack(const Value& value) = 0;
    virtual Value PopFromStack() = 0;

    virtual Value LoadVariable(std::string_view name) const = 0;
    virtual void StoreVariable(std::string_view name, const Value& value) = 0;

    virtual Value ReadInput() = 0;
    virtual void WriteOutput(const Value& value) = 0;

    virtual void Jump(const Label& label) = 0;

    virtual ~IExecutionContext() = default;

protected:
    IExecutionContext() = default;
};
