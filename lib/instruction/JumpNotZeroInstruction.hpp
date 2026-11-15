#pragma once

#include "IInstruction.hpp"
#include "Label.hpp"

class JumpNotZeroInstruction final : public IInstruction {
public:
    JumpNotZeroInstruction(Label label);

    void Execute(IExecutionContext& context) const override;

private:
    Label label_;
};
