#pragma once

#include "runtime/env.h"
#include "runtime/object.h"

#include <memory>
#include <string>
#include <unordered_map>

class Evaluator;

class SpecialForm {
public:
    using ObjectPtr = std::shared_ptr<Object>;
    using EnvPtr = std::shared_ptr<Environment>;

    virtual ~SpecialForm() = default;

    virtual ObjectPtr Evaluate(const ObjectPtr& args, const EnvPtr& env, Evaluator& evaluator) = 0;
};

using SpecialFormPtr = std::shared_ptr<SpecialForm>;

class SpecialFormRegistry {
public:
    void Register(const std::string& name, SpecialFormPtr form);

    SpecialFormPtr Lookup(const std::string& name) const;

private:
    std::unordered_map<std::string, SpecialFormPtr> forms_;
};

SpecialFormRegistry CreateStandardForms();
