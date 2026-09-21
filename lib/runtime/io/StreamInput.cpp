#include "StreamInput.hpp"

#include <cstdint>
#include <istream>

#include "error/InvalidInput.hpp"

StreamInput::StreamInput(std::istream& in) : in_(in) {}

Value StreamInput::Read() {
    Value value;
    if (!(in_ >> value)) {
        throw InvalidInput();
    }

    return value;
}