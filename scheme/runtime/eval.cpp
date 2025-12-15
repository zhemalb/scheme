#include "eval.h"

#include "error.h"
#include "object.h"
#include "procedure.h"
#include "special_forms.h"

#include <utility>
#include <vector>

Evaluator::Evaluator() : special_forms_(CreateStandardForms()) {
}

Evaluator::Evaluator(SpecialFormRegistry special_forms) : special_forms_(std::move(special_forms)) {
}

ObjectPtr Evaluator::Eval(const ObjectPtr& expr, const EnvPtr& env) {
    if (!env) {
        throw RuntimeError{"Cannot evaluate with empty environment"};
    }
    if (!expr) {
        throw RuntimeError{"Cannot evaluate empty list"};
    }
    if (Is<Number>(expr) || Is<Boolean>(expr)) {
        return expr;
    }
    if (auto symbol = As<Symbol>(expr)) {
        return env->Lookup(symbol->GetName());
    }

    auto cell = As<Cell>(expr);
    if (!cell) {
        throw RuntimeError{"Invalid expression"};
    }

    auto head = cell->GetFirst();
    auto tail = cell->GetSecond();

    if (auto sym = As<Symbol>(head)) {
        if (auto form = special_forms_.Lookup(sym->GetName())) {
            return form->Evaluate(tail, env, *this);
        }
    }

    auto proc_obj = Eval(head, env);
    auto proc = As<Procedure>(proc_obj);
    if (!proc) {
        throw RuntimeError{"Not a procedure"};
    }
    std::vector<std::shared_ptr<Object>> arg_values;
    std::shared_ptr<Object> cur = tail;
    while (cur) {
        auto arg_cell = As<Cell>(cur);
        if (!arg_cell) {
            throw RuntimeError{"Expected proper list"};
        }
        arg_values.push_back(Eval(arg_cell->GetFirst(), env));
        cur = arg_cell->GetSecond();
    }
    return proc->Apply(arg_values, env, *this);
}
