#pragma once

#include "IMachine.hpp"
#include "runtime/environment/IValueStore.hpp"
#include "runtime/io/IInput.hpp"
#include "runtime/io/IOutput.hpp"

class Machine final : public IMachine {
public:
    Machine(IValueStorePtr variables, IInputPtr input, IOutputPtr output);

    void Execute(const Program& program) const override;

private:
    IValueStorePtr variables_;
    IInputPtr input_;
    IOutputPtr output_;
};
