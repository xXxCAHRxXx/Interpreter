#pragma once

#include <iosfwd>

#include "runtime/io/IInput.hpp"

class StreamInput final : public IInput {
public:
    explicit StreamInput(std::istream& in);

    Value Read() override;

private:
    std::istream& in_;
};