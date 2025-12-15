#include "procedure.h"

#include "eval.h"

Procedure::ObjectPtr LambdaProcedure::Apply(const ArgsVec& args, const EnvPtr&,
                                            Evaluator& evaluator) {
    if (args.size() != params_.size()) {
        throw RuntimeError{"Invalid argument count"};
    }
    auto call_env = std::make_shared<Environment>(closure_);
    for (auto i = 0; i < params_.size(); ++i) {
        call_env->Define(params_[i], args[i]);
    }

    ObjectPtr result = nullptr;
    for (const auto& expr : body_) {
        result = evaluator.Eval(expr, call_env);
    }
    return result;
}
