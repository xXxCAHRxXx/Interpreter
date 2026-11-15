#pragma once

#include <iosfwd>

#include "backend/runtime/io/IOutput.hpp"

class StreamOutput final : public IOutput {
public:
    explicit StreamOutput(std::ostream& out);

    void Write(const Value& value) override;

private:
    std::ostream& out_;
};
