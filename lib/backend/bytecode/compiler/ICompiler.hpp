#pragma once

#include <memory>

#include <gsl/pointers>

#include "frontend/ast/statement/IStmt.hpp"
#include "backend/bytecode/program/Program.hpp"

class ICompiler {
public:
    ICompiler(const ICompiler&) = delete;
    ICompiler& operator=(const ICompiler&) = delete;

    virtual ProgramPtr Compile(IStmtPtr statement) = 0;

    virtual ~ICompiler() = default;

protected:
    ICompiler() = default;
};

using ICompilerPtr = gsl::not_null<std::shared_ptr<ICompiler>>;
