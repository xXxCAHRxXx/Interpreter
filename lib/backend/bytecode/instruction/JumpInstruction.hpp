#pragma once

#include "IInstruction.hpp"
#include "Label.hpp"

class JumpInstruction final : public IInstruction {
public:
    JumpInstruction(Label label);

    void Execute(IExecutionContext& context) const override;

private:
    Label label_;
};
