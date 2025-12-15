#pragma once

#include <memory>
#include <string>

class Object : public std::enable_shared_from_this<Object> {
public:
    virtual ~Object() = default;
};

class Boolean : public Object {
public:
    explicit Boolean(bool value) : value_(value) {
    }

    bool GetValue() const {
        return value_;
    }

private:
    bool value_;
};

class Number : public Object {
public:
    Number(int64_t value);
    int64_t GetValue() const;

private:
    int64_t value_;
};

class Symbol : public Object {
public:
    Symbol(std::string name);
    const std::string& GetName() const;

private:
    std::string name_;
};

class Cell : public Object {
public:
    Cell(std::shared_ptr<Object> first, std::shared_ptr<Object> second);

    std::shared_ptr<Object> GetFirst() const;
    std::shared_ptr<Object> GetSecond() const;

    void SetFirst(std::shared_ptr<Object> first);
    void SetSecond(std::shared_ptr<Object> second);

private:
    std::shared_ptr<Object> first_;
    std::shared_ptr<Object> second_;
};

template <class T>
std::shared_ptr<T> As(const std::shared_ptr<Object>& obj) {
    return std::dynamic_pointer_cast<T>(obj);
}

template <class T>
bool Is(const std::shared_ptr<Object>& obj) {
    return As<T>(obj) != nullptr;
}

inline std::shared_ptr<Boolean> True() {
    static auto value = std::make_shared<Boolean>(true);
    return value;
}

inline std::shared_ptr<Boolean> False() {
    static auto value = std::make_shared<Boolean>(false);
    return value;
}

inline std::shared_ptr<Boolean> MakeBool(bool value) {
    return value ? True() : False();
}
