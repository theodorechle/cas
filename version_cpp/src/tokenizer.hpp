#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

#include <string>
#include "constants.hpp"
#include "node.hpp"
#include "functions.hpp"

class UnknownValue: public std::exception {
    std::string value;
public:
    UnknownValue(const std::string& value): value{value} {};
    const char* what() const noexcept override {return ("Error : Unknown value '" + value + "'").c_str();};
};

bool isTypeOrEmpty(constants::Types type, constants::Types checkType);

bool isperator(std::string &value);

void addTreeByValues(Node &t, std::string *value, constants::Types type);


bool tokenizeSpace(char character, std::string &value, bool &createNewTree);

bool tokenizeAlpha(char character, std::string &value, bool &createNewTree, int &index, constants::Types &type, Node *exprList, bool implicitPriority);

bool tokenizeOpeningParenthesis(char character, std::string &value, bool &createNewTree, int &index, constants::Types &type, Node *exprList, bool implicitPriority);

bool tokenizeClosingParenthesis(char character, std::string &value, int &index, constants::Types &type);

bool tokenizeNumber(char character, std::string &value, bool &createNewTree, int &index, constants::Types &type);

bool tokenizeOperator(char character, std::string &value, bool &createNewTree, int &index, constants::Types &yytype, std::string &testString);

Node *tokenizer(const std::string &expr, bool debug, bool implicitPriority);

#endif // TOKENIZER_HPP