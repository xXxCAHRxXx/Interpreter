#pragma once

#include "IValueReader.hpp"

class IValueStore : public IValueReader {
public:
    virtual void Set(std::string_view name, const Value& value) = 0;
};

using IValueStorePtr = gsl::not_null<std::shared_ptr<IValueStore>>;