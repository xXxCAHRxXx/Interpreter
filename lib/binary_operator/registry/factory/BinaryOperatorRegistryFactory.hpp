#pragma once

#include "IBinaryOperatorRegistryFactory.hpp"

class BinaryOperatorRegistryFactory final : public IBinaryOperatorRegistryFactory {
public:
    BinaryOperatorRegistry Create() const override;
};