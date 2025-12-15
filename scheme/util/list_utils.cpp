#include "list_utils.h"

#include "error.h"

namespace listutils {

using ObjectPtr = ObjectPtr;

bool IsProperList(const ObjectPtr& obj) {
    ObjectPtr cur = obj;
    while (cur) {
        auto cell = As<Cell>(cur);
        if (!cell) {
            return false;
        }
        cur = cell->GetSecond();
    }
    return true;
}

ObjectVec ToVector(const ObjectPtr& list) {
    ObjectVec out;
    ObjectPtr cur = list;
    while (cur) {
        auto cell = As<Cell>(cur);
        if (!cell) {
            throw RuntimeError{"Expected proper list"};
        }
        out.push_back(cell->GetFirst());
        cur = cell->GetSecond();
    }
    return out;
}

ObjectPtr FromVector(const ObjectVec& vec) {
    ObjectPtr head = nullptr;
    std::shared_ptr<Cell> last = nullptr;
    for (const auto& elem : vec) {
        auto cell = std::make_shared<Cell>(elem, nullptr);
        if (!head) {
            head = cell;
            last = cell;
        } else {
            last->SetSecond(cell);
            last = cell;
        }
    }
    return head;
}

ObjectPtr Advance(ObjectPtr list, int64_t steps) {
    if (!IsProperList(list)) {
        throw RuntimeError{"Expected proper list"};
    }
    ObjectPtr cur = std::move(list);
    for (auto i = 0; i < steps; ++i) {
        auto cell = As<Cell>(cur);
        if (!cell) {
            throw RuntimeError{"Index out of range"};
        }
        cur = cell->GetSecond();
    }
    return cur;
}

}  // namespace listutils
