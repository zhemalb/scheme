#include "tokenizer.h"

#include <string>

enum class Char { Whitespace, SymbolStart, SymbolBody, Other };

Char Classify(char ch) {
    unsigned char uch = static_cast<unsigned char>(ch);

    if (std::isspace(uch)) {
        return Char::Whitespace;
    }

    std::string_view start = "<=>*#";
    std::string_view body  = "<=>*#?!-";

    if (std::isalpha(uch) || start.find(ch) != std::string_view::npos) {
        return Char::SymbolStart;
    }
    if (std::isalpha(uch) || std::isdigit(uch) || body.find(ch) != std::string_view::npos) {
        return Char::SymbolBody;
    }
    return Char::Other;
}

bool IsStart(char ch) {
    return Classify(ch) == Char::SymbolStart;
}

bool IsBody(char ch) {
    unsigned char uch = static_cast<unsigned char>(ch);
    if (std::isdigit(uch)) {
        return true;
    }
    Char cls = Classify(ch);
    return cls == Char::SymbolStart || cls == Char::SymbolBody;
}

void SkipWhitespace(std::istream& in) {
    while (true) {
        int64_t c = in.peek();
        if (c == EOF) {
            break;
        }
        char ch = static_cast<char>(c);
        if (!std::isspace(static_cast<unsigned char>(ch))) {
            break;
        }
        in.get();
    }
}

int64_t ReadNum(std::istream& in, int64_t sign, char first_digit) {
    std::string digits;
    digits.push_back(first_digit);

    while (true) {
        int64_t c = in.peek();
        if (c == EOF) {
            break;
        }
        char ch = static_cast<char>(c);
        if (!std::isdigit(static_cast<unsigned char>(ch))) {
            break;
        }
        digits.push_back(ch);
        in.get();
    }

    int64_t value = 0;
    if (!digits.empty()) {
        value = static_cast<int64_t>(std::stoll(digits));
    }
    return sign * value;
}

std::string ReadSymbol(std::istream& in, char first_char) {
    std::string name;
    name.push_back(first_char);

    while (true) {
        int64_t c = in.peek();
        if (c == EOF) {
            break;
        }
        char ch = static_cast<char>(c);
        if (!IsBody(ch)) {
            break;
        }
        name.push_back(ch);
        in.get();
    }

    return name;
}

bool Tokenizer::ReadToken(Token& out) {
    while (true) {
        SkipWhitespace(*in_);

        int64_t c = in_->peek();
        if (c == EOF) {
            return false;
        }

        char ch = static_cast<char>(c);

        if (ch == '(') {
            in_->get();
            out = BracketToken::OPEN;
            return true;
        } else if (ch == ')') {
            in_->get();
            out = BracketToken::CLOSE;
            return true;
        } else if (ch == '\'') {
            in_->get();
            out = QuoteToken{};
            return true;
        } else if (ch == '.') {
            in_->get();
            out = DotToken{};
            return true;
        } else if (ch == '/') {
            in_->get();
            out = SymbolToken{"/"};
            return true;
        } else if (std::isdigit(static_cast<unsigned char>(ch))) {
            in_->get();
            int64_t num = ReadNum(*in_, +1, ch);
            out = ConstantToken{num};
            return true;
        } else if (ch == '+' || ch == '-') {
            in_->get();
            int64_t next = in_->peek();
            if (next != EOF && std::isdigit(static_cast<unsigned char>(next))) {
                char first_digit = static_cast<char>(next);
                in_->get();
                int64_t sign = (ch == '-') ? -1 : 1;
                int64_t num = ReadNum(*in_, sign, first_digit);
                out = ConstantToken{num};
                return true;
            } else {
                out = SymbolToken{std::string(1, ch)};
                return true;
            }
        } else if (IsStart(ch)) {
            in_->get();
            std::string symbol = ReadSymbol(*in_, ch);
            out = SymbolToken{std::move(symbol)};
            return true;
        }
    }
}
