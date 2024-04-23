#include "tokenizer.hpp"

using namespace std;

bool isCharInValues(char c, char *values, int nbValues) {
    for (int i=0; i<nbValues; i++) {
        if (c == values[i]) return true;
    }
    return false;
}

int tokenizeSpace(const string &expr, int index) {
    int i = 0;
    while (isspace(expr[index+i])) {
        i++;
    }
    return i;
}

int tokenizeName(const string &expr, int index, Node *tokens) {
    if (!isalpha(expr[index]) ||
            expr[index] == '_') {
        return 0;
    }
    int i = 1;
    while (isalnum(expr[index+i]) ||
            expr[index] == '_') {
        i++;
    }
    tokens->appendNext(new Node{expr.substr(index, i), Token::Name});
    return i;
}

int tokenizeNumber(const string &expr, int index, Node *tokens) {
    if (!isdigit(expr[index])) return 0;
    bool dotFound = false;
    int i = 1;
    while (isdigit(expr[index+i]) ||
            expr[index+i] == '_' ||
            (!dotFound && expr[index] == '.')) {
        i++;
    }
    tokens->appendNext(new Node{expr.substr(index, i), Token::Number});
    return i;
    
}

int tokenizeSpecialCharacters(const string &expr, int index, Node *tokens) {
    if (expr[index] == '*' && expr[index+1] == '*') {
        tokens->appendNext(new Node{expr[index] + expr[index+1], Token::DoubleTimes});
        return 2;
    }
    Token token;
    switch (expr[index]) {
    case '(':
        token = Token::OpeningParenthesis;
        break;
    case ')':
        token = Token::ClosingParenthesis;
        break;
    case '+':
        token = Token::Plus;
        break;
    case '-':
        token = Token::Minus;
        break;
    case '*':
        token = Token::Times;
        break;
    case '/':
        token = Token::Slash;
        break;
    case '^':
        token = Token::Caret;
        break;
    case ',':
        token = Token::Comma;
        break;
    case '!':
        token = Token::Bang;
        break;    
    default:
        token = Token::Empty;
        break;
    }
    if (token == Token::Empty) return 0;
    tokens->appendNext(new Node{expr[index], token});
    return 1;
}

Node *tokenizer(const string &expr) {
    Node *exprList = new Node{};
    Token type = Token::Empty;
    int i = 0;
    for (int index=0; index<expr.length(); index++) {
        i = tokenizeSpace(expr, index, exprList);
        if (!i) i = tokenizeNumber(expr, index, exprList);
        if (!i) i = tokenizeName(expr, index, exprList);
        if (!i) i = tokenizeSpecialCharacters(expr, index, exprList);
        if (!i) throw UnknownValue(expr.substr(index));
    }
   return exprList;
}