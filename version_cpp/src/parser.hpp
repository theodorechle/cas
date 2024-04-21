#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include <exception>

#include "constants.hpp"
#include "node.hpp"

class UnknownToken: public std::exception {
    Node token;
public:
    UnknownToken(const Node token): token{token} {};
    const char* what() const noexcept override;
};

void removeParenthesis(Node *t);

Node *findRootOrParenthesis(Node *tree);

/**
  *Transform a chain of trees (no childs) like the one the parser function returns into a tree containing the entire expression
*/
Node *parser(Node *exprList, bool debug, bool implicitPriority);

#endif // PARSER_HPP
