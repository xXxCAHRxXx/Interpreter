#pragma once

#include <memory>

#include <gsl/pointers>

#include "backend/bytecode/machine/IExecutionContext.hpp"

class IInstruction {
public:
    IInstruction(const IInstruction&) = delete;
    IInstruction& operator=(const IInstruction&) = delete;

    virtual void Execute(IExecutionContext& context) const = 0;

    virtual ~IInstruction() = default;

protected:
    IInstruction() = default;
};

using IInstructionPtr = gsl::not_null<std::shared_ptr<IInstruction>>;
