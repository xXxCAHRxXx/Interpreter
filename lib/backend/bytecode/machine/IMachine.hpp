#pragma once

#include <memory>

#include <gsl/pointers>

#include "backend/bytecode/program/Program.hpp"

class IMachine {
public:
    IMachine(const IMachine&) = delete;
    IMachine& operator=(const IMachine&) = delete;

    virtual void Execute(ProgramPtr program) const = 0;

    virtual ~IMachine() = default;

protected:
    IMachine() = default;
};

using IMachinePtr = gsl::not_null<std::shared_ptr<IMachine>>;
