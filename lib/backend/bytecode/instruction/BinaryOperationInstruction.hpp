#pragma once

#include "IInstruction.hpp"
#include "binary_operator/IBinaryOperator.hpp"

class BinaryOperationInstruction final : public IInstruction {
public:
    BinaryOperationInstruction(IBinaryOperatorPtr op);

    void Execute(IExecutionContext& context) const override;

private:
    IBinaryOperatorPtr operator_;
};
