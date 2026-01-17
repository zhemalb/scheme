#include "eval/special_forms.h"

#include "eval/eval.h"
#include "eval/procedure.h"
#include "runtime/error.h"
#include "runtime/helpers.h"
#include "runtime/list_utils.h"

#include <utility>
#include <vector>

namespace {

using ObjectPtr = SpecialForm::ObjectPtr;
using EnvPtr = SpecialForm::EnvPtr;
using CellPtr = std::shared_ptr<Cell>;
using ArgsVec = std::vector<ObjectPtr>;
using FormPtr = SpecialFormPtr;

std::vector<std::string> ParseParamNames(const ObjectPtr& params_obj) {
    std::vector<std::string> params;
    ObjectPtr cur = params_obj;
    while (cur) {
        auto cell = As<Cell>(cur);
        if (!cell) {
            throw SyntaxError{""};
        }
        auto sym = As<Symbol>(cell->GetFirst());
        if (!sym) {
            throw SyntaxError{""};
        }
        params.push_back(sym->GetName());
        cur = cell->GetSecond();
    }
    return params;
}

ArgsVec ToVectorOrSyntaxError(const ObjectPtr& list) {
    try {
        return listutils::ToVector(list);
    } catch (const RuntimeError&) {
        throw SyntaxError{""};
    }
}

class QuoteForm : public SpecialForm {
public:
    ObjectPtr Evaluate(const ObjectPtr& args, const EnvPtr&, Evaluator&) override {
        auto vec = ToVectorOrSyntaxError(args);
        if (vec.size() != 1) {
            throw SyntaxError{""};
        }
        return vec[0];
    }
};

class IfForm : public SpecialForm {
public:
    ObjectPtr Evaluate(const ObjectPtr& args, const EnvPtr& env, Evaluator& evaluator) override {
        auto vec = ToVectorOrSyntaxError(args);
        if (vec.size() != 2 && vec.size() != 3) {
            throw SyntaxError{""};
        }
        auto cond = evaluator.Eval(vec[0], env);
        if (!helpers::IsFalse(cond)) {
            return evaluator.Eval(vec[1], env);
        }
        if (vec.size() == 3) {
            return evaluator.Eval(vec[2], env);
        }
        return nullptr;
    }
};

class LambdaForm : public SpecialForm {
public:
    ObjectPtr Evaluate(const ObjectPtr& args, const EnvPtr& env, Evaluator&) override {
        auto vec = ToVectorOrSyntaxError(args);
        if (vec.size() < 2) {
            throw SyntaxError{""};
        }
        auto params = ParseParamNames(vec[0]);
        ArgsVec body(vec.begin() + 1, vec.end());
        return std::make_shared<LambdaProcedure>(std::move(params), std::move(body), env);
    }
};

class DefineForm : public SpecialForm {
public:
    ObjectPtr Evaluate(const ObjectPtr& args, const EnvPtr& env, Evaluator& evaluator) override {
        auto vec = ToVectorOrSyntaxError(args);
        if (vec.size() < 2) {
            throw SyntaxError{""};
        }

        if (auto name = As<Symbol>(vec[0])) {
            if (vec.size() != 2) {
                throw SyntaxError{""};
            }
            auto value = evaluator.Eval(vec[1], env);
            env->Define(name->GetName(), std::move(value));
            return nullptr;
        }

        auto signature = As<Cell>(vec[0]);
        if (!signature) {
            throw SyntaxError{""};
        }
        auto name = As<Symbol>(signature->GetFirst());
        if (!name) {
            throw SyntaxError{""};
        }
        auto params_obj = signature->GetSecond();
        auto params = ParseParamNames(params_obj);
        ArgsVec body(vec.begin() + 1, vec.end());
        auto lambda = std::make_shared<LambdaProcedure>(std::move(params), std::move(body), env);
        env->Define(name->GetName(), std::move(lambda));
        return nullptr;
    }
};

class SetForm : public SpecialForm {
public:
    ObjectPtr Evaluate(const ObjectPtr& args, const EnvPtr& env, Evaluator& evaluator) override {
        auto vec = ToVectorOrSyntaxError(args);
        if (vec.size() != 2) {
            throw SyntaxError{""};
        }
        auto name = As<Symbol>(vec[0]);
        if (!name) {
            throw SyntaxError{""};
        }
        auto value = evaluator.Eval(vec[1], env);
        env->Set(name->GetName(), std::move(value));
        return nullptr;
    }
};

class AndForm : public SpecialForm {
public:
    ObjectPtr Evaluate(const ObjectPtr& args, const EnvPtr& env, Evaluator& evaluator) override {
        if (!args) {
            return True();
        }
        ObjectPtr cur = args;
        ObjectPtr last = True();
        while (cur) {
            auto cell = As<Cell>(cur);
            if (!cell) {
                throw SyntaxError{""};
            }
            last = evaluator.Eval(cell->GetFirst(), env);
            if (helpers::IsFalse(last)) {
                return False();
            }
            cur = cell->GetSecond();
        }
        return last;
    }
};

class OrForm : public SpecialForm {
public:
    ObjectPtr Evaluate(const ObjectPtr& args, const EnvPtr& env, Evaluator& evaluator) override {
        if (!args) {
            return False();
        }
        ObjectPtr cur = args;
        ObjectPtr last = False();
        while (cur) {
            auto cell = As<Cell>(cur);
            if (!cell) {
                throw SyntaxError{""};
            }
            last = evaluator.Eval(cell->GetFirst(), env);
            if (!helpers::IsFalse(last)) {
                return last;
            }
            cur = cell->GetSecond();
        }
        return last;
    }
};

}  // namespace

void SpecialFormRegistry::Register(const std::string& name, FormPtr form) {
    forms_[name] = std::move(form);
}

SpecialFormPtr SpecialFormRegistry::Lookup(const std::string& name) const {
    auto it = forms_.find(name);
    if (it != forms_.end()) {
        return it->second;
    }
    return nullptr;
}

SpecialFormRegistry CreateStandardForms() {
    SpecialFormRegistry registry;
    registry.Register("quote", std::make_shared<QuoteForm>());
    registry.Register("if", std::make_shared<IfForm>());
    registry.Register("lambda", std::make_shared<LambdaForm>());
    registry.Register("define", std::make_shared<DefineForm>());
    registry.Register("set!", std::make_shared<SetForm>());
    registry.Register("and", std::make_shared<AndForm>());
    registry.Register("or", std::make_shared<OrForm>());
    return registry;
}
