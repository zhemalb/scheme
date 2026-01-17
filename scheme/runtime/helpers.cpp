#include "runtime/helpers.h"

#include "runtime/error.h"

namespace helpers {

int64_t RequireInt(const ObjectPtr& obj) {
    auto num = As<Number>(obj);
    if (!num) {
        throw RuntimeError{"Expected number"};
    }
    return num->GetValue();
}

CellPtr RequireCell(const ObjectPtr& obj) {
    auto cell = As<Cell>(obj);
    if (!cell) {
        throw RuntimeError{"Expected pair"};
    }
    return cell;
}

int64_t RequireIndex(const ObjectPtr& obj) {
    auto index = RequireInt(obj);
    if (index < 0) {
        throw RuntimeError{"Invalid index"};
    }
    return index;
}

const Args& RequireArgsCount(const Args& args, size_t n) {
    if (args.size() != n) {
        throw RuntimeError{"Invalid argument count"};
    }
    return args;
}

bool IsFalse(const ObjectPtr& obj) {
    auto boolean = As<Boolean>(obj);
    return boolean && !boolean->GetValue();
}

}  // namespace helpers
