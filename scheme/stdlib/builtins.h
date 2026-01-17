#pragma once

#include "runtime/env.h"

#include <memory>

void AddBuiltins(const std::shared_ptr<Environment>& env);
