#pragma once

#include "runtime/object.h"

#include <memory>
#include <vector>

namespace listutils {

using ObjectPtr = std::shared_ptr<Object>;
using ObjectVec = std::vector<ObjectPtr>;

bool IsProperList(const ObjectPtr& obj);

ObjectVec ToVector(const ObjectPtr& list);

ObjectPtr FromVector(const ObjectVec& vec);

ObjectPtr Advance(ObjectPtr list, int64_t steps);

}  // namespace listutils
