#include "stdlib/int_operations.h"

#include "eval/procedure.h"
#include "runtime/helpers.h"

#include <algorithm>
#include <functional>

using helpers::Args;
using helpers::NumericChainCmp;
using helpers::NumericFold;
using helpers::RequireArgsCount;
using helpers::RequireInt;
using helpers::UnaryPredicate;

using EnvPtr = std::shared_ptr<Environment>;
using ProcPtr = std::shared_ptr<BuiltinProcedure>;

namespace {

template <class Fn>
std::shared_ptr<Object> NumericFoldWrapper(const Args& args, const EnvPtr&, Evaluator&, Fn fn,
                                           int64_t identity, bool require_at_least_one) {
    return NumericFold(args, identity, require_at_least_one, fn);
}

std::shared_ptr<Object> AddFn(const Args& args, const EnvPtr& env, Evaluator& ev) {
    return NumericFoldWrapper(args, env, ev, std::plus<int64_t>{}, 0, false);
}

std::shared_ptr<Object> MulFn(const Args& args, const EnvPtr& env, Evaluator& ev) {
    return NumericFoldWrapper(args, env, ev, std::multiplies<int64_t>{}, 1, false);
}

std::shared_ptr<Object> SubFn(const Args& args, const EnvPtr&, Evaluator&) {
    if (args.empty()) {
        throw RuntimeError{"Invalid argument count"};
    }
    auto value = RequireInt(args[0]);
    if (args.size() == 1) {
        return std::make_shared<Number>(-value);
    }
    for (auto i = 1; i < args.size(); ++i) {
        value -= RequireInt(args[i]);
    }
    return std::make_shared<Number>(value);
}

std::shared_ptr<Object> DivFn(const Args& args, const EnvPtr&, Evaluator&) {
    if (args.empty()) {
        throw RuntimeError{"Invalid argument count"};
    }
    auto value = RequireInt(args[0]);
    if (args.size() == 1) {
        throw RuntimeError{"Invalid argument count"};
    }
    for (auto i = 1; i < args.size(); ++i) {
        auto div = RequireInt(args[i]);
        value /= div;
    }
    return std::make_shared<Number>(value);
}

template <class Pred>
std::shared_ptr<Object> ComparisonFn(const Args& args, const EnvPtr&, Evaluator&, Pred pred) {
    return NumericChainCmp(args, pred);
}

std::shared_ptr<Object> EqFn(const Args& args, const EnvPtr& env, Evaluator& ev) {
    return ComparisonFn(args, env, ev, std::equal_to<int64_t>{});
}

std::shared_ptr<Object> LtFn(const Args& args, const EnvPtr& env, Evaluator& ev) {
    return ComparisonFn(args, env, ev, std::less<int64_t>{});
}

std::shared_ptr<Object> GtFn(const Args& args, const EnvPtr& env, Evaluator& ev) {
    return ComparisonFn(args, env, ev, std::greater<int64_t>{});
}

std::shared_ptr<Object> LeFn(const Args& args, const EnvPtr& env, Evaluator& ev) {
    return ComparisonFn(args, env, ev, std::less_equal<int64_t>{});
}

std::shared_ptr<Object> GeFn(const Args& args, const EnvPtr& env, Evaluator& ev) {
    return ComparisonFn(args, env, ev, std::greater_equal<int64_t>{});
}

std::shared_ptr<Object> MaxFn(const Args& args, const EnvPtr&, Evaluator&) {
    if (args.empty()) {
        throw RuntimeError{"Invalid argument count"};
    }
    auto best = RequireInt(args[0]);
    for (auto i = 1; i < args.size(); ++i) {
        best = std::max(best, RequireInt(args[i]));
    }
    return std::make_shared<Number>(best);
}

std::shared_ptr<Object> MinFn(const Args& args, const EnvPtr&, Evaluator&) {
    if (args.empty()) {
        throw RuntimeError{"Invalid argument count"};
    }
    auto best = RequireInt(args[0]);
    for (auto i = 1; i < args.size(); ++i) {
        best = std::min(best, RequireInt(args[i]));
    }
    return std::make_shared<Number>(best);
}

std::shared_ptr<Object> AbsFn(const Args& args, const EnvPtr&, Evaluator&) {
    RequireArgsCount(args, 1);
    auto v = RequireInt(args[0]);
    return std::make_shared<Number>(v < 0 ? -v : v);
}

ProcPtr MakeProc(std::shared_ptr<Object> (*fn)(const Args&, const EnvPtr&, Evaluator&)) {
    return std::make_shared<BuiltinProcedure>(fn);
}

}  // namespace

void RegisterIntOperations(const std::shared_ptr<Environment>& env) {
    env->Define("number?",
                std::make_shared<BuiltinProcedure>([](const auto& args, const auto&, auto&) {
                    return UnaryPredicate(args, Is<Number>);
                }));
    env->Define("+", MakeProc(&AddFn));
    env->Define("*", MakeProc(&MulFn));
    env->Define("-", MakeProc(&SubFn));
    env->Define("/", MakeProc(&DivFn));

    env->Define("=", MakeProc(&EqFn));
    env->Define("<", MakeProc(&LtFn));
    env->Define(">", MakeProc(&GtFn));
    env->Define("<=", MakeProc(&LeFn));
    env->Define(">=", MakeProc(&GeFn));

    env->Define("max", MakeProc(&MaxFn));
    env->Define("min", MakeProc(&MinFn));
    env->Define("abs", MakeProc(&AbsFn));
}
