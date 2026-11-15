#include "Skip.hpp"

#include "frontend/ast/statement/visitor/IStmtVisitor.hpp"

void Skip::Accept(IStmtVisitor& visitor) const {
    visitor.Visit(*this);
}
