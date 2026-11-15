#pragma once

#include "IInstruction.hpp"

class WriteInstruction final : public IInstruction {
public:
    void Execute(IExecutionContext& context) const override;
};
