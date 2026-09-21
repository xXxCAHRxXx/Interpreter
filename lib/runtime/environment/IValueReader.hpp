#pragma once

#include <memory>
#include <string_view>

#include <gsl/pointers>

#include "value/Value.hpp"

class IValueReader {
public:
    IValueReader(const IValueReader&) = delete;
    IValueReader& operator=(const IValueReader&) = delete;

    virtual Value Get(std::string_view name) const = 0;

    virtual ~IValueReader() = default;

protected:
    IValueReader() = default;
};

using IValueReaderPtr = gsl::not_null<std::shared_ptr<IValueReader>>;