#include "io/printer.h"

#include "runtime/error.h"

#include <sstream>

namespace {

void PrintList(std::ostringstream& out, const std::shared_ptr<Object>& list) {
    std::shared_ptr<Object> cur = list;
    bool first = true;
    while (cur) {
        auto cell = As<Cell>(cur);
        if (!cell) {
            out << " . " << Print(cur);
            return;
        }
        if (!first) {
            out << ' ';
        }
        first = false;
        out << Print(cell->GetFirst());
        cur = cell->GetSecond();
    }
}

}  // namespace

std::string Print(const std::shared_ptr<Object>& obj) {
    if (!obj) {
        return "()";
    }
    if (auto boolean = As<Boolean>(obj)) {
        return boolean->GetValue() ? "#t" : "#f";
    }
    if (auto number = As<Number>(obj)) {
        return std::to_string(number->GetValue());
    }
    if (auto symbol = As<Symbol>(obj)) {
        return symbol->GetName();
    }
    if (auto cell = As<Cell>(obj)) {
        std::ostringstream out;
        out << '(';
        PrintList(out, obj);
        out << ')';
        return out.str();
    }
    throw RuntimeError{"Unknown object"};
}
