#pragma once

#include <memory>

#include <gsl/pointers>

#include "interpreter/IInterpreter.hpp"

class IInterpreterFactory {
public:
    IInterpreterFactory(const IInterpreterFactory&) = delete;
    IInterpreterFactory& operator=(const IInterpreterFactory&) = delete;

    virtual IInterpreterPtr Create() const = 0;

    virtual ~IInterpreterFactory() = default;

protected:
    IInterpreterFactory() = default;
};

using IInterpreterFactoryPtr = gsl::not_null<std::shared_ptr<IInterpreterFactory>>;
