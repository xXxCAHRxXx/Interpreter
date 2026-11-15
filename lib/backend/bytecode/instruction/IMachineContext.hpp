#pragma once

#include <memory>
#include <string_view>

#include <gsl/pointers>

#include "value/Value.hpp"

class IMachineContext {
public:
    IMachineContext(const IMachineContext&) = delete;
    IMachineContext& operator=(const IMachineContext&) = delete;

    virtual void PushToStack(const Value& value) = 0;
    virtual Value PopFromStack() = 0;

    virtual Value LoadVariable(std::string_view name) const = 0;
    virtual void StoreVariable(std::string_view name, const Value& value) = 0;

    virtual Value ReadInput() = 0;
    virtual void WriteOutput(const Value& value) = 0;

    virtual ~IMachineContext() = default;

protected:
    IMachineContext() = default;
};

using IMachineContextPtr = gsl::not_null<std::shared_ptr<IMachineContext>>;
