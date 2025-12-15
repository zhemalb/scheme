#pragma once

#include "error.h"
#include "object.h"

#include <memory>
#include <vector>

namespace helpers {

using ObjectPtr = std::shared_ptr<Object>;
using CellPtr = std::shared_ptr<Cell>;
using Args = std::vector<ObjectPtr>;

int64_t RequireInt(const ObjectPtr& obj);

CellPtr RequireCell(const ObjectPtr& obj);

int64_t RequireIndex(const ObjectPtr& obj);

const Args& RequireArgsCount(const Args& args, size_t n);

bool IsFalse(const ObjectPtr& obj);

template <class Pred>
std::shared_ptr<Object> UnaryPredicate(const Args& args, Pred pred) {
    RequireArgsCount(args, 1);
    return MakeBool(pred(args[0]));
}

template <class Fn>
std::shared_ptr<Object> NumericFold(const Args& args, int64_t identity, bool require_alo, Fn fn) {
    if (require_alo && args.empty()) {
        throw RuntimeError{"Invalid argument count"};
    }
    auto acc = identity;
    for (const auto& a : args) {
        acc = fn(acc, RequireInt(a));
    }
    return std::make_shared<Number>(acc);
}

template <class Pred>
std::shared_ptr<Object> NumericChainCmp(const Args& args, Pred pred) {
    if (args.size() <= 1) {
        return True();
    }
    auto prev = RequireInt(args[0]);
    for (auto i = 1; i < args.size(); ++i) {
        auto cur = RequireInt(args[i]);
        if (!pred(prev, cur)) {
            return False();
        }
        prev = cur;
    }
    return True();
}

}  // namespace helpers
