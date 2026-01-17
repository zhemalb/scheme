#pragma once

#include "runtime/env.h"
#include "runtime/object.h"

#include <functional>
#include <memory>
#include <string>
#include <vector>

class Evaluator;

class Procedure : public Object {
public:
    using ObjectPtr = std::shared_ptr<Object>;
    using EnvPtr = std::shared_ptr<Environment>;
    using ArgsVec = std::vector<ObjectPtr>;
    using Params = std::vector<std::string>;

    virtual ObjectPtr Apply(const ArgsVec& args, const EnvPtr& env, Evaluator& evaluator) = 0;
};

class BuiltinProcedure final : public Procedure {
public:
    using Fn =
        std::function<ObjectPtr(const ArgsVec& args, const EnvPtr& env, Evaluator& evaluator)>;

    explicit BuiltinProcedure(Fn fn) : fn_(std::move(fn)) {
    }

    ObjectPtr Apply(const ArgsVec& args, const EnvPtr& env, Evaluator& evaluator) override {
        return fn_(args, env, evaluator);
    }

private:
    Fn fn_;
};

using ProcPtr = std::shared_ptr<BuiltinProcedure>;

class LambdaProcedure final : public Procedure {
public:
    LambdaProcedure(Params params, ArgsVec body, EnvPtr closure)
        : params_(std::move(params)), body_(std::move(body)), closure_(std::move(closure)) {
    }

    ObjectPtr Apply(const ArgsVec& args, const EnvPtr& env, Evaluator& evaluator) override;

private:
    Params params_;
    ArgsVec body_;
    EnvPtr closure_;
};
