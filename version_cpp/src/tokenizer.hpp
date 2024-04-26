#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

#include <string>

#include "node.hpp"

class UnknownValue: public std::exception {
    std::string message;
public:
    UnknownValue(const std::string& value): message{"Error : Unknown value '" + value + "'"} {};
    const char* what() const noexcept override {return message.c_str();};
};

int tokenizeSpace(const std::string &expr, int index);

int tokenizeName(const std::string &expr, int index, Node *tokens);

int tokenizeNumber(const std::string &expr, int index, Node *tokens);

int tokenizeSpecialCharacters(const std::string &expr, int index, Node *tokens);

Node *tokenizer(const std::string &expr);

#endif // TOKENIZER_HPP