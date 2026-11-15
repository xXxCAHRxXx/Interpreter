#pragma once

#include "IMachine.hpp"
#include "backend/runtime/environment/IValueStore.hpp"
#include "backend/runtime/io/IInput.hpp"
#include "backend/runtime/io/IOutput.hpp"

class Machine final : public IMachine {
public:
    Machine(IValueStorePtr variables, IInputPtr input, IOutputPtr output);

    void Execute(ProgramPtr program) const override;

private:
    IValueStorePtr variables_;
    IInputPtr input_;
    IOutputPtr output_;
};
