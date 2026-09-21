#pragma once

#include <memory>
#include <gsl/pointers>

class IExprVisitor;

class IExpr {
    public:
    IExpr(const IExpr&) = delete;
    IExpr& operator=(const IExpr&) = delete;
    
    virtual void Accept(IExprVisitor& visitor) const = 0;
    
    virtual ~IExpr() = default;
    
    protected:
    IExpr() = default;
};

using IExprPtr = gsl::not_null<std::shared_ptr<IExpr>>;