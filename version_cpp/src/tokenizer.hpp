#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

#include <string>

#include "node.hpp"
#include "functions.hpp"
#include "tokens.hpp"

class UnknownValue: public std::exception {
    std::string value;
public:
    UnknownValue(const std::string& value): value{value} {};
    const char* what() const noexcept override {return ("Error : Unknown value '" + value + "'").c_str();};
};

int tokenizeSpace(const string &expr, int index);

int tokenizeName(const string &expr, int index, Node *tokens);

int tokenizeNumber(const string &expr, int index, Node *tokens);

int tokenizeSpecialCharacters(const string &expr, int index, Node *tokens);

Node *tokenizer(const std::string &expr);

#endif // TOKENIZER_HPP