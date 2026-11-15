#pragma once

#include "IInstruction.hpp"

class ReadInstruction final : public IInstruction {
public:
    void Execute(IExecutionContext& context) const override;
};
