#include "stdlib/builtins.h"

#include "stdlib/bool_operations.h"
#include "stdlib/int_operations.h"
#include "stdlib/list_operations.h"

void AddBuiltins(const std::shared_ptr<Environment>& env) {
    env->Define("#t", True());
    env->Define("#f", False());
    RegisterBoolOperations(env);
    RegisterIntOperations(env);
    RegisterListOperations(env);
}
