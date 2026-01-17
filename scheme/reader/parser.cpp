#include "reader/parser.h"

#include "runtime/error.h"
#include "runtime/object.h"

std::shared_ptr<Object> ReadList(Tokenizer* tokenizer);

void ThrowSyntax() {
    throw SyntaxError{""};
}

std::shared_ptr<Object> ReadInternal(Tokenizer* tokenizer) {
    Token token = tokenizer->GetToken();
    if (ConstantToken* number = std::get_if<ConstantToken>(&token)) {
        tokenizer->Next();
        return std::make_shared<Number>(number->value);
    }
    if (SymbolToken* symbol = std::get_if<SymbolToken>(&token)) {
        tokenizer->Next();
        if (symbol->name == "#t") {
            return True();
        }
        if (symbol->name == "#f") {
            return False();
        }
        return std::make_shared<Symbol>(symbol->name);
    }
    if (BracketToken* bracket = std::get_if<BracketToken>(&token)) {
        if (*bracket != BracketToken::OPEN) {
            ThrowSyntax();
        }
        tokenizer->Next();
        return ReadList(tokenizer);
    }
    if (std::holds_alternative<QuoteToken>(token)) {
        tokenizer->Next();
        if (tokenizer->IsEnd()) {
            ThrowSyntax();
        }
        auto quoted = ReadInternal(tokenizer);
        auto quote_sym = std::make_shared<Symbol>("quote");
        return std::make_shared<Cell>(quote_sym, std::make_shared<Cell>(quoted, nullptr));
    }
    if (std::holds_alternative<DotToken>(token)) {
        ThrowSyntax();
    }
    return nullptr;
}

std::shared_ptr<Object> ReadList(Tokenizer* tokenizer) {
    std::shared_ptr<Object> first;
    std::shared_ptr<Cell> last;

    for (;;) {
        Token token = tokenizer->GetToken();
        if (BracketToken* bracket = std::get_if<BracketToken>(&token)) {
            if (*bracket == BracketToken::CLOSE) {
                tokenizer->Next();
                return first;
            }
        }

        std::shared_ptr<Object> elem = ReadInternal(tokenizer);
        if (tokenizer->IsEnd()) {
            ThrowSyntax();
        }

        Token next = tokenizer->GetToken();
        if (std::holds_alternative<DotToken>(next)) {
            tokenizer->Next();
            std::shared_ptr<Object> last_val = ReadInternal(tokenizer);

            Token closing = tokenizer->GetToken();
            BracketToken* bracket = std::get_if<BracketToken>(&closing);
            if (!bracket) {
                ThrowSyntax();
            }
            tokenizer->Next();
            std::shared_ptr<Cell> final = std::make_shared<Cell>(elem, last_val);
            if (!first) {
                return final;
            }
            last->SetSecond(final);
            return first;
        }

        std::shared_ptr<Cell> appended = std::make_shared<Cell>(elem, nullptr);
        if (!first) {
            first = appended;
            last = appended;
        } else {
            last->SetSecond(appended);
            last = appended;
        }
    }
}

std::shared_ptr<Object> Read(Tokenizer* tokenizer) {
    if (tokenizer->IsEnd()) {
        ThrowSyntax();
    }
    std::shared_ptr<Object> result = ReadInternal(tokenizer);
    if (!tokenizer->IsEnd()) {
        ThrowSyntax();
    }
    return result;
}
