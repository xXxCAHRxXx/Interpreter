#pragma once

#include <iosfwd>
#include <memory>

#include <gsl/pointers>

class IInterpreter {
public:
    IInterpreter(const IInterpreter&) = delete;
    IInterpreter& operator=(const IInterpreter&) = delete;

    virtual void Run(std::istream& program) const = 0;

    virtual ~IInterpreter() = default;

protected:
    IInterpreter() = default;
};

using IInterpreterPtr = gsl::not_null<std::shared_ptr<IInterpreter>>;
