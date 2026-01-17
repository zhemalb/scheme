#pragma once

#include "eval/special_forms.h"

#include <memory>

class Environment;
class Object;

using ObjectPtr = std::shared_ptr<Object>;
using EnvPtr = std::shared_ptr<Environment>;

class Evaluator {
public:
    Evaluator();
    explicit Evaluator(SpecialFormRegistry special_forms);

    ObjectPtr Eval(const ObjectPtr& expr, const EnvPtr& env);

private:
    SpecialFormRegistry special_forms_;
};
