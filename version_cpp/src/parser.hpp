#ifndef PARSER_HPP
#define PARSER_HPP

#include <exception>
#include <unordered_map>

#include "number.hpp"
#include "settings.hpp"

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

class Parser {
    Node *expressionTokens;
    Node *expressionTreeRoot = new Node{Token::NullRoot};
    Node *expressionTree = expressionTreeRoot;
    Settings *settings;
    bool debug;
    bool implicitMultiplicationPriority;

    void getRootOrStopBeforeParenthesis();
    void removeParenthesis(Node *t);
    void replaceImplicitTimes(Node *t);

public:
    Parser(Node *expressionTokens, Settings *settings, bool debug=false, bool implicitMultiplicationPriority=false)
    : expressionTokens{expressionTokens}, settings{settings}, debug{debug}, implicitMultiplicationPriority{implicitMultiplicationPriority} {};
    
    /**
     * Transform a chain of trees (no childs) like the one the parser function returns into a tree containing the entire expression
    */
    Node *parse();

    void parseNumber();
    void parseVariable();
    void parseOperator();
    void parseOpeningParenthesis();
    void parseClosingParenthesis();
    void addImplicitMultiplication();
    static bool isNodeNull(Node *node);
};

#endif // PARSER_HPP