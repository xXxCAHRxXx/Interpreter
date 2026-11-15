#include "Label.hpp"

#include <utility>

Label::Label(std::string name)
: name_(std::move(name)) { }

const std::string& Label::GetName() const noexcept {
    return name_;
}
