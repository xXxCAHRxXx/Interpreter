#pragma once

#include <memory>

#include <gsl/pointers>

#include "binary_operator/registry/BinaryOperatorRegistry.hpp"

class IBinaryOperatorRegistryFactory {
public:
    IBinaryOperatorRegistryFactory(const IBinaryOperatorRegistryFactory&) = delete;
    IBinaryOperatorRegistryFactory& operator=(const IBinaryOperatorRegistryFactory&) = delete;

    virtual BinaryOperatorRegistry Create() const = 0;

    virtual ~IBinaryOperatorRegistryFactory() = default;

protected:
    IBinaryOperatorRegistryFactory() = default;
};

using IBinaryOperatorRegistryFactoryPtr = gsl::not_null<std::shared_ptr<IBinaryOperatorRegistryFactory>>;
