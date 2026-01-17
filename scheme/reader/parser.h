#pragma once

#include "reader/tokenizer.h"
#include "runtime/object.h"

#include <memory>

std::shared_ptr<Object> Read(Tokenizer* tokenizer);
