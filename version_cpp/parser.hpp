#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include <exception>

#include "constants.hpp"
#include "node.hpp"

class NullError : public std::exception {
    std::string msg;
public:
    NullError(std::string msg): msg{msg} {};
    const char *what() const noexcept override {std::string m = "Unknown value '"+msg+"'"; return m.c_str();};
};

bool isTypeOrEmpty(constants::Types type, constants::Types checkType);

bool isperator(std::string &value);


void addTreeByValues(Node &t, std::string *value, constants::Types type);

Node *parser(std::string &expr, bool debug, bool implicitPriority);

Node *findRootOrParenthesis(Node *tree);

/**
  *Transform a chain of trees (no childs) like the one the parser function returns into a tree containing the entire expression
*/
Node *parsedToTree(Node *exprList, bool debug, bool implicitPriority);

#endif // PARSER_HPP