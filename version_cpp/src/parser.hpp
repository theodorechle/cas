#ifndef PARSER_HPP
#define PARSER_HPP

#include <exception>
#include <unordered_map>

#include "number.hpp"

int getOperatorPriority(const Token &token);

class UnknownToken: public std::exception {
    std::string message;
public:
    UnknownToken(const Node token): message{"Error : Unknown token '" + token.getValue() + " (" + tokenToStr(token.getTokenType()) + ")'"} {};
    const char* what() const noexcept override {return message.c_str();}
};

class MissingToken: public std::exception {
    std::string message;
public:
    MissingToken(const std::string &token): message{"Error : Missing token '" + token + "'"} {};
    const char* what() const noexcept override {return message.c_str();}
};

class InvalidExpression: public std::exception {
    std::string message;
public:
    InvalidExpression(const std::string &expression): message{"Error : Invalid expression '" + expression + "'"} {};
    const char *what() const noexcept override {return message.c_str();}
};

void removeParenthesis(Node *t);

Node *getRootOrStopBeforeParenthesis(Node *tree);

bool isNodeNull(Node *node);

/**
  * Transform a chain of trees (no childs) like the one the parser function returns into a tree containing the entire expression
*/
Node *parser(Node *exprList, bool debug, bool implicitPriority);

Node *parseNumber(Node *tree, Node *token);
Node *parseVariable(Node *tree, Node *token);
Node *parseOperator(Node *tree, Node *token);
Node *parseOpeningParenthesis(Node *tree, Node *token);
Node *parseClosingParenthesis(Node *tree, Node *token);
Node *addImplicitMultiplication(Node *tree, Node *token);

#endif // PARSER_HPP