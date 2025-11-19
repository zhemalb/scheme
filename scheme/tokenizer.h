#pragma once

#include <cstddef>
#include <istream>
#include <string>
#include <variant>
#include <vector>

struct SymbolToken {
    std::string name;
    bool operator==(const SymbolToken& other) const {
        return name == other.name;
    }
};

struct QuoteToken {
    bool operator==(const QuoteToken&) const {
        return true;
    }
};

struct DotToken {
    bool operator==(const DotToken&) const {
        return true;
    }
};

enum class BracketToken { OPEN, CLOSE };

struct ConstantToken {
    int64_t value;
    bool operator==(const ConstantToken& other) const {
        return value == other.value;
    }
};

using Token = std::variant<ConstantToken, BracketToken, SymbolToken, QuoteToken, DotToken>;

class Tokenizer {
public:
    Tokenizer(std::istream* in) {
        in_ = std::move(in);
        tokens_ = {};
        pos_  = 0;
        end_reached_ = false;
    }

    bool IsEnd() {
        return !EnsureToken(pos_);
    }

    void Next() {
        if (!IsEnd()) {
            ++pos_;
        }
    }

    Token GetToken() {
        if (!EnsureToken(pos_)) {
            return SymbolToken{""};
        }
        return tokens_[pos_];
    }

private:
    using TokenList = std::vector<Token>;

    std::istream* in_ = nullptr;
    TokenList tokens_;
    size_t pos_ = 0;
    bool end_reached_ = false;

    bool ReadToken(Token& out);
    bool EnsureToken(size_t index) {
        while (tokens_.size() <= index && !end_reached_) {
            Token token;
            if (ReadToken(token)) {
                tokens_.push_back(std::move(token));
            } else {
                end_reached_ = true;
            }
        }
        return index < tokens_.size();
    }
};
