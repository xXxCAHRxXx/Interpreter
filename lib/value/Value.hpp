#pragma once

#include <cstdint>
#include <iosfwd>

class Value {
public:
    Value() noexcept;
    explicit Value(int32_t value) noexcept;
    explicit Value(bool value) noexcept;

    explicit operator bool() const noexcept;

    Value operator+(const Value& other) const;
    Value operator-(const Value& other) const;
    Value operator*(const Value& other) const;
    Value operator/(const Value& other) const;
    Value operator%(const Value& other) const;

    bool operator==(const Value& other) const noexcept;
    bool operator!=(const Value& other) const noexcept;
    bool operator<(const Value& other) const noexcept;
    bool operator>(const Value& other) const noexcept;
    bool operator<=(const Value& other) const noexcept;
    bool operator>=(const Value& other) const noexcept;

    friend std::ostream& operator<<(std::ostream& out, const Value& other);
    friend std::istream& operator>>(std::istream& in, Value& other);

private:
    int32_t value_;
};