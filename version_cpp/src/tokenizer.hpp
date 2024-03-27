#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

#include <string>
#include "constants.hpp"
#include "node.hpp"
#include "functions.hpp"

class NullError : public std::exception {
    std::string msg;
public:
    NullError(const std::string &msg): msg{msg} {};
    const char *what() const noexcept override {std::string m = "Unknown value '"+msg+"'"; return m.c_str();};
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