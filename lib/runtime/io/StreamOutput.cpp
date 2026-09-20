#include "StreamOutput.hpp"

#include <ostream>

StreamOutput::StreamOutput(std::ostream& out)
: out_(out)
{ }

void StreamOutput::Write(const Value& value) {
    out_ << value << '\n';
}