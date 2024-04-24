#ifndef PARSER_HPP
#define PARSER_HPP

#include <exception>
#include <unordered_map>

#include "number.hpp"

int getOperatorPriority(const Token &token);

class UnknownToken: public std::exception {
    Node token;
public:
    UnknownToken(const Node token): token{token} {};
    const char* what() const noexcept override;
};

class MissingToken: public std::exception {
    std::string token;
public:
    MissingToken(const std::string &token): token{token} {};
    const char* what() const noexcept override;
};

void removeParenthesis(Node *t);

Node *getRootOrParenthesis(Node *tree);

/**
  * Transform a chain of trees (no childs) like the one the parser function returns into a tree containing the entire expression
*/
Node *parser(Node *exprList, bool debug, bool implicitPriority);

Node *parseNumber(Node *tree, Node *token);
Node *parseVariable(Node *tree, Node *token);
Node *parseOperator(Node *tree, Node *token);
Node *parseOpeningParenthesis(Node *tree, Node *token);
Node *parseClosingParenthesis(Node *tree, Node *token);

#endif // PARSER_HPP