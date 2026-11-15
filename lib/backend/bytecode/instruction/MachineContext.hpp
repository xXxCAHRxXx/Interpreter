#pragma once

#include <string_view>
#include <vector>

#include "backend/bytecode/instruction/IMachineContext.hpp"
#include "backend/runtime/environment/IValueStore.hpp"
#include "backend/runtime/io/IInput.hpp"
#include "backend/runtime/io/IOutput.hpp"

class MachineContext final : public IMachineContext {
public:
    MachineContext(IValueStorePtr variables, IInputPtr input, IOutputPtr output);

    void PushToStack(const Value& value) override;
    Value PopFromStack() override;

    Value LoadVariable(std::string_view name) const override;
    void StoreVariable(std::string_view name, const Value& value) override;

    Value ReadInput() override;
    void WriteOutput(const Value& value) override;

private:
    IValueStorePtr variables_;
    IInputPtr input_;
    IOutputPtr output_;

    std::vector<Value> stack_;
};
