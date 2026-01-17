#include "stdlib/bool_operations.h"

#include "eval/procedure.h"
#include "runtime/helpers.h"
#include "runtime/list_utils.h"

using helpers::IsFalse;
using helpers::RequireArgsCount;
using helpers::UnaryPredicate;

namespace {

template <class Pred>
ProcPtr MakePredicate(Pred pred) {
    return std::make_shared<BuiltinProcedure>(
        [pred](const auto& args, const auto&, auto&) { return UnaryPredicate(args, pred); });
}

ProcPtr MakeNot() {
    return std::make_shared<BuiltinProcedure>([](const auto& args, const auto&, auto&) {
        RequireArgsCount(args, 1);
        return MakeBool(IsFalse(args[0]));
    });
}

}  // namespace

void RegisterBoolOperations(const std::shared_ptr<Environment>& env) {
    env->Define("boolean?", MakePredicate(Is<Boolean>));
    env->Define("symbol?", MakePredicate(Is<Symbol>));
    env->Define("pair?", MakePredicate(Is<Cell>));
    env->Define("null?", MakePredicate([](const auto& obj) { return obj == nullptr; }));
    env->Define("list?", MakePredicate(listutils::IsProperList));
    env->Define("not", MakeNot());
}
