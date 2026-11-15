#pragma once

#include <cstddef>
#include <string>
#include <unordered_map>
#include <vector>

#include "backend/bytecode/instruction/IInstruction.hpp"
#include "backend/bytecode/instruction/Label.hpp"

class Program {
public:
    using Iterator = std::vector<IInstructionPtr>::const_iterator;

    class Builder {
    public:
        Builder& AddInstruction(IInstructionPtr instruction);
        Builder& AddLabel(const Label& label);

        Program Build();

    private:
        std::vector<IInstructionPtr> instructions_;
        std::unordered_map<std::string, std::size_t> labels_;
    };

    Iterator begin() const noexcept;
    Iterator end() const noexcept;
    Iterator Find(const Label& label) const;

private:
    Program(std::vector<IInstructionPtr> instructions, std::unordered_map<std::string, std::size_t> labels);

    std::vector<IInstructionPtr> instructions_;
    std::unordered_map<std::string, std::size_t> labels_;
};

using ProgramPtr = gsl::not_null<std::shared_ptr<const Program>>;
