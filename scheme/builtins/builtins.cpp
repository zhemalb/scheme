#include "builtins.h"

#include "bool_operations.h"
#include "int_operations.h"
#include "list_operations.h"

void AddBuiltins(const std::shared_ptr<Environment>& env) {
    env->Define("#t", True());
    env->Define("#f", False());
    RegisterBoolOperations(env);
    RegisterIntOperations(env);
    RegisterListOperations(env);
}
