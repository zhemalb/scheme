#include "object.h"

#include <utility>

Number::Number(int64_t value) {
    value_ = std::move(value);
}

int64_t Number::GetValue() const {
    return value_;
}

Symbol::Symbol(std::string name) {
    name_ = std::move(name);
}

const std::string& Symbol::GetName() const {
    return name_;
}

Cell::Cell(std::shared_ptr<Object> first, std::shared_ptr<Object> second) {
    first_ = std::move(first);
    second_ = std::move(second);
}

std::shared_ptr<Object> Cell::GetFirst() const {
    return first_;
}

std::shared_ptr<Object> Cell::GetSecond() const {
    return second_;
}

void Cell::SetFirst(std::shared_ptr<Object> first) {
    first_ = std::move(first);
}

void Cell::SetSecond(std::shared_ptr<Object> second) {
    second_ = std::move(second);
}
