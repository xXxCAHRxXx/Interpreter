#pragma once

#include <string_view>
#include <vector>

#include "backend/bytecode/machine/IExecutionContext.hpp"
#include "backend/bytecode/program/Program.hpp"
#include "backend/runtime/environment/IValueStore.hpp"
#include "backend/runtime/io/IInput.hpp"
#include "backend/runtime/io/IOutput.hpp"

class ExecutionContext final : public IExecutionContext {
public:
    ExecutionContext(ProgramPtr program, IValueStorePtr variables, IInputPtr input, IOutputPtr output);

    bool HasNext() const;
    IInstructionPtr Next();

    void PushToStack(const Value& value) override;
    Value PopFromStack() override;

    Value LoadVariable(std::string_view name) const override;
    void StoreVariable(std::string_view name, const Value& value) override;

    Value ReadInput() override;
    void WriteOutput(const Value& value) override;

    void Jump(const Label& label) override;

private:
    ProgramPtr program_;
    IValueStorePtr variables_;
    IInputPtr input_;
    IOutputPtr output_;

    std::vector<Value> stack_;
    Program::Iterator current_;
};
