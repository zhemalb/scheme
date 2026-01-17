#pragma once

#include "eval/eval.h"

#include <memory>
#include <string>

class Environment;

class Scheme {
public:
    Scheme();
    ~Scheme();
    std::string Evaluate(const std::string& expression);

private:
    Evaluator evaluator_;
    std::shared_ptr<Environment> global_env_;
};
