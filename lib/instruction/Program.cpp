#include "Program.hpp"

#include <utility>

#include "error/LogicError.hpp"

Program::Builder& Program::Builder::AddInstruction(IInstructionPtr instruction) {
    instructions_.push_back(std::move(instruction));

    return *this;
}

Program::Builder& Program::Builder::AddLabel(const Label& label) {
    bool inserted = labels_.emplace(label.GetName(), instructions_.size()).second;

    if (!inserted) {
        throw LogicError("Program::Builder::AddLabel: label is already added");
    }

    return *this;
}

Program Program::Builder::Build() {
    return Program(std::exchange(instructions_, {}), std::exchange(labels_, {}));
}

Program::Iterator Program::begin() const noexcept {
    return instructions_.begin();
}

Program::Iterator Program::end() const noexcept {
    return instructions_.end();
}

Program::Iterator Program::Find(const Label& label) const {
    auto entry = labels_.find(label.GetName());

    if (entry == labels_.end()) {
        throw LogicError("Program::Find: label is not added");
    }

    return instructions_.begin() + entry->second;
}

Program::Program(std::vector<IInstructionPtr> instructions, std::unordered_map<std::string, std::size_t> labels)
: instructions_(std::move(instructions))
, labels_(std::move(labels)) { }
