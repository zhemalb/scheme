#pragma once

#include "error.h"
#include "object.h"

#include <memory>
#include <string>
#include <unordered_map>

class Environment : public std::enable_shared_from_this<Environment> {
public:
    using Ptr = std::shared_ptr<Environment>;
    using ObjectPtr = std::shared_ptr<Object>;
    using ValuesMap = std::unordered_map<std::string, ObjectPtr>;

    explicit Environment(Ptr parent = nullptr) : parent_(std::move(parent)) {
    }

    void Define(const std::string& name, ObjectPtr value) {
        values_[name] = std::move(value);
    }

    ObjectPtr Lookup(const std::string& name) const {
        auto it = values_.find(name);
        if (it != values_.end()) {
            return it->second;
        }
        if (parent_) {
            return parent_->Lookup(name);
        }
        throw NameError{name};
    }

    void Set(const std::string& name, ObjectPtr value) {
        auto it = values_.find(name);
        if (it != values_.end()) {
            it->second = std::move(value);
            return;
        }
        if (parent_) {
            parent_->Set(name, std::move(value));
            return;
        }
        throw NameError{name};
    }

    void Clear() {
        values_.clear();
        parent_.reset();
    }

private:
    Ptr parent_;
    ValuesMap values_;
};
