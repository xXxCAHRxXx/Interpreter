#pragma once

#include <string>

#include "IInstruction.hpp"

class LoadInstruction final : public IInstruction {
public:
    LoadInstruction(std::string name);

    void Execute(IExecutionContext& context) const override;

private:
    std::string name_;
};
