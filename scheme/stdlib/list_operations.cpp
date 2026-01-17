#include "stdlib/list_operations.h"

#include "eval/procedure.h"
#include "runtime/helpers.h"
#include "runtime/list_utils.h"
#include "runtime/object.h"

#include <memory>

using helpers::Args;
using helpers::RequireArgsCount;
using helpers::RequireCell;
using helpers::RequireIndex;

using EnvPtr = std::shared_ptr<Environment>;
using ProcPtr = std::shared_ptr<BuiltinProcedure>;

namespace {

std::shared_ptr<Object> ConsFn(const Args& args, const EnvPtr&, Evaluator&) {
    RequireArgsCount(args, 2);
    return std::make_shared<Cell>(args[0], args[1]);
}

std::shared_ptr<Object> ListFn(const Args& args, const EnvPtr&, Evaluator&) {
    return listutils::FromVector(args);
}

std::shared_ptr<Object> CarFn(const Args& args, const EnvPtr&, Evaluator&) {
    RequireArgsCount(args, 1);
    return RequireCell(args[0])->GetFirst();
}

std::shared_ptr<Object> CdrFn(const Args& args, const EnvPtr&, Evaluator&) {
    RequireArgsCount(args, 1);
    return RequireCell(args[0])->GetSecond();
}

std::shared_ptr<Object> SetCarFn(const Args& args, const EnvPtr&, Evaluator&) {
    RequireArgsCount(args, 2);
    RequireCell(args[0])->SetFirst(args[1]);
    return nullptr;
}

std::shared_ptr<Object> SetCdrFn(const Args& args, const EnvPtr&, Evaluator&) {
    RequireArgsCount(args, 2);
    RequireCell(args[0])->SetSecond(args[1]);
    return nullptr;
}

std::shared_ptr<Object> ListRefFn(const Args& args, const EnvPtr&, Evaluator&) {
    RequireArgsCount(args, 2);
    auto idx = RequireIndex(args[1]);
    auto cur = listutils::Advance(args[0], idx);
    auto cell = As<Cell>(cur);
    if (!cell) {
        throw RuntimeError{"Index out of range"};
    }
    return cell->GetFirst();
}

std::shared_ptr<Object> ListTailFn(const Args& args, const EnvPtr&, Evaluator&) {
    RequireArgsCount(args, 2);
    auto idx = RequireIndex(args[1]);
    auto cur = listutils::Advance(args[0], idx);
    if (!cur) {
        return nullptr;
    }
    if (!listutils::IsProperList(cur)) {
        throw RuntimeError{"Expected proper list"};
    }
    return cur;
}

ProcPtr MakeProc(std::shared_ptr<Object> (*fn)(const Args&, const EnvPtr&, Evaluator&)) {
    return std::make_shared<BuiltinProcedure>(fn);
}

}  // namespace

void RegisterListOperations(const std::shared_ptr<Environment>& env) {
    env->Define("cons", MakeProc(&ConsFn));
    env->Define("list", MakeProc(&ListFn));
    env->Define("car", MakeProc(&CarFn));
    env->Define("cdr", MakeProc(&CdrFn));
    env->Define("set-car!", MakeProc(&SetCarFn));
    env->Define("set-cdr!", MakeProc(&SetCdrFn));
    env->Define("list-ref", MakeProc(&ListRefFn));
    env->Define("list-tail", MakeProc(&ListTailFn));
}
