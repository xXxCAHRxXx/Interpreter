#include "ExecutionContext.hpp"

#include <utility>

#include "error/StackUnderflow.hpp"

ExecutionContext::ExecutionContext(ProgramPtr program, IValueStorePtr variables, IInputPtr input, IOutputPtr output)
: program_(std::move(program))
, variables_(std::move(variables))
, input_(std::move(input))
, output_(std::move(output))
, current_(program_->begin()) { }

bool ExecutionContext::HasNext() const {
    return current_ != program_->end();
}

IInstructionPtr ExecutionContext::Next() {
    IInstructionPtr instruction = *current_;
    ++current_;

    return instruction;
}

void ExecutionContext::PushToStack(const Value& value) {
    stack_.push_back(value);
}

Value ExecutionContext::PopFromStack() {
    if (stack_.empty()) {
        throw StackUnderflow();
    }

    Value value = stack_.back();
    stack_.pop_back();

    return value;
}

Value ExecutionContext::LoadVariable(std::string_view name) const {
    return variables_->Get(name);
}

void ExecutionContext::StoreVariable(std::string_view name, const Value& value) {
    variables_->Set(name, value);
}

Value ExecutionContext::ReadInput() {
    return input_->Read();
}

void ExecutionContext::WriteOutput(const Value& value) {
    output_->Write(value);
}

void ExecutionContext::Jump(const Label& label) {
    current_ = program_->Find(label);
}
