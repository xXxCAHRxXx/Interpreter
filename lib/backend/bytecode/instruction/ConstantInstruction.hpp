#pragma once

#include "IInstruction.hpp"
#include "value/Value.hpp"

class ConstantInstruction final : public IInstruction {
public:
    ConstantInstruction(const Value& value);

    void Execute(IExecutionContext& context) const override;

private:
    Value value_;
};
