#pragma once

#include <string>

#include "IInstruction.hpp"

class StoreInstruction final : public IInstruction {
public:
    StoreInstruction(std::string name);

    void Execute(IExecutionContext& context) const override;

private:
    std::string name_;
};
