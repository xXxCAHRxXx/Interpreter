#pragma once

#include <string_view>
#include <vector>

#include "instruction/IExecutionContext.hpp"
#include "instruction/Program.hpp"
#include "runtime/environment/IValueStore.hpp"
#include "runtime/io/IInput.hpp"
#include "runtime/io/IOutput.hpp"

class ExecutionContext final : public IExecutionContext {
public:
    ExecutionContext(const Program& program, IValueStorePtr variables, IInputPtr input, IOutputPtr output);

    bool HasNext() const;
    const IInstruction& Next();

    void PushToStack(const Value& value) override;
    Value PopFromStack() override;

    Value LoadVariable(std::string_view name) const override;
    void StoreVariable(std::string_view name, const Value& value) override;

    Value ReadInput() override;
    void WriteOutput(const Value& value) override;

    void Jump(const Label& label) override;

private:
    const Program& program_;
    IValueStorePtr variables_;
    IInputPtr input_;
    IOutputPtr output_;

    std::vector<Value> stack_;
    Program::Iterator current_;
};
