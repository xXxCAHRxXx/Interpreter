#include "MachineContext.hpp"

#include <utility>

#include "error/StackUnderflow.hpp"

MachineContext::MachineContext(IValueStorePtr variables, IInputPtr input, IOutputPtr output)
: variables_(std::move(variables))
, input_(std::move(input))
, output_(std::move(output)) { }

void MachineContext::PushToStack(const Value& value) {
    stack_.push_back(value);
}

Value MachineContext::PopFromStack() {
    if (stack_.empty()) {
        throw StackUnderflow();
    }

    Value value = stack_.back();
    stack_.pop_back();

    return value;
}

Value MachineContext::LoadVariable(std::string_view name) const {
    return variables_->Get(name);
}

void MachineContext::StoreVariable(std::string_view name, const Value& value) {
    variables_->Set(name, value);
}

Value MachineContext::ReadInput() {
    return input_->Read();
}

void MachineContext::WriteOutput(const Value& value) {
    output_->Write(value);
}
