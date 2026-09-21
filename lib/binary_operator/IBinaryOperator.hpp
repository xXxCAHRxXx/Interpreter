#pragma once

#include <memory>

#include <gsl/pointers>

#include "value/Value.hpp"

class IBinaryOperator {
public:
    IBinaryOperator(const IBinaryOperator&) = delete;
    IBinaryOperator& operator=(const IBinaryOperator&) = delete;
    
    virtual Value Apply(const Value& lhs, const Value& rhs) const = 0;
    
    virtual ~IBinaryOperator() = default;

protected:
    IBinaryOperator() = default;
};

using IBinaryOperatorPtr = gsl::not_null<std::shared_ptr<IBinaryOperator>>;