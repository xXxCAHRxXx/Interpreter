#include "Value.hpp"

#include <istream>
#include <ostream>

#include "error/DivisionByZero.hpp"

Value::Value() noexcept
: value_(0)
{ }

Value::Value(int32_t value) noexcept 
: value_(value) 
{ }

Value::Value(bool value) noexcept
: value_(value ? 1 : 0)
{ }

Value::operator bool() const noexcept {
    return value_ != 0;
}

Value Value::operator+(const Value& other) const {
    return Value(value_ + other.value_);
}

Value Value::operator-(const Value& other) const {
    return Value(value_ - other.value_);
}

Value Value::operator*(const Value& other) const {  
    return Value(value_ * other.value_);
}

Value Value::operator/(const Value& other) const {
    if (other.value_ == 0) {
        throw DivisionByZero();
    }

    return Value(value_ / other.value_);
}

Value Value::operator%(const Value& other) const {
    if (other.value_ == 0) {
        throw DivisionByZero();
    }

    return Value(value_ % other.value_);
}

bool Value::operator==(const Value& other) const noexcept {
    return value_ == other.value_;
}

bool Value::operator!=(const Value& other) const noexcept {
    return !(*this == other);
}

bool Value::operator<(const Value& other) const noexcept {
    return value_ < other.value_;
}

bool Value::operator>(const Value& other) const noexcept {
    return other < *this;
}

bool Value::operator<=(const Value& other) const noexcept {
    return !(*this > other);
}

bool Value::operator>=(const Value& other) const noexcept {
    return !(*this < other);
}

std::ostream& operator<<(std::ostream& out, const Value& other) {
    return out << other.value_;
}

std::istream& operator>>(std::istream& in, Value& other) {
    return in >> other.value_;
}
