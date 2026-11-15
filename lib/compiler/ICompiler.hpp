#pragma once

#include <memory>

#include <gsl/pointers>

#include "ast/statement/IStmt.hpp"
#include "instruction/Program.hpp"

class ICompiler {
public:
    ICompiler(const ICompiler&) = delete;
    ICompiler& operator=(const ICompiler&) = delete;

    virtual Program Compile(const IStmt& statement) = 0;

    virtual ~ICompiler() = default;

protected:
    ICompiler() = default;
};

using ICompilerPtr = gsl::not_null<std::shared_ptr<ICompiler>>;
