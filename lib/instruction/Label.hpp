#pragma once

#include <string>

class Label final {
public:
    Label(std::string name);

    const std::string& GetName() const noexcept;

private:
    std::string name_;
};
