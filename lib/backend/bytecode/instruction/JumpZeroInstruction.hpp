#pragma once

#include "IInstruction.hpp"
#include "Label.hpp"

class JumpZeroInstruction final : public IInstruction {
public:
    JumpZeroInstruction(Label label);

    void Execute(IExecutionContext& context) const override;

private:
    Label label_;
};
