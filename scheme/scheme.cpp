#include "scheme.h"

#include "eval/eval.h"
#include "io/printer.h"
#include "reader/parser.h"
#include "reader/tokenizer.h"
#include "runtime/env.h"
#include "stdlib/builtins.h"

#include <sstream>

Scheme::Scheme() : global_env_(std::make_shared<Environment>()) {
    AddBuiltins(global_env_);
}

Scheme::~Scheme() {
    if (global_env_) {
        global_env_->Clear();
    }
}

std::string Scheme::Evaluate(const std::string& expression) {
    std::istringstream in(expression);
    Tokenizer tokenizer(&in);
    auto ast = Read(&tokenizer);
    auto value = evaluator_.Eval(ast, global_env_);
    return Print(value);
}
