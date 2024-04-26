using namespace std;

#include "parser.hpp"

void removeParenthesis(Node *t) {
    if (t->getTokenType() == Token::ClosingParenthesis) t->replaceData(t->getChild());
    else if (t->getTokenType() == Token::OpeningParenthesis) throw MissingToken(")");
    Node *child = t->getChild();
    while (child != nullptr) {
        removeParenthesis(child);
        child = child->getNext();
    }
}

Node *getRootOrStopBeforeParenthesis(Node *tree) {
    if (tree == nullptr ||
            isNodeNull(tree->getParent()) ||
            tree->getParent()->getTokenType() == Token::OpeningParenthesis)
        return tree;
    return getRootOrStopBeforeParenthesis(tree->getParent());
}

bool isNodeNull(Node *node) {
    return (node == nullptr || node->getTokenType() == Token::NullRoot);
}

Node *parser(Node *tokenList, bool debug, bool implicitPriority) {
    Node *tree = new Node{Token::NullRoot};
    while (tokenList != nullptr) {
        Token tokenType = tokenList->getTokenType();
        if (tokenType == Token::Number) {
            tree = parseNumber(tree, tokenList);
        }
        else if (tokenType == Token::Name) {
            tree = parseVariable(tree, tokenList);
        }
        else if (tokenType == Token::Plus ||
                tokenType == Token::Minus ||
                tokenType == Token::Times ||
                tokenType == Token::Slash ||
                tokenType == Token::Caret ||
                tokenType == Token::DoubleTimes) {
            tree = parseOperator(tree, tokenList);
        }
        else if (tokenType == Token::OpeningParenthesis) {
            tree = parseOpeningParenthesis(tree, tokenList);
        }
        else if (tokenType == Token::ClosingParenthesis) {
            tree = parseClosingParenthesis(tree, tokenList);
        }
        else {
            delete root(tree);
            throw UnknownToken(*tokenList);
        }
        if (debug) {
            cerr << endl << "Root : " << endl;
            root(tree)->display(cerr);
            cerr << endl;
        }
        tokenList = tokenList->getNext();
    }

    tree = root(tree);
    removeParenthesis(tree);
    deleteNullRoot(tree);
    return tree;
}

Node *parseNumber(Node *tree, Node *token) {
    tree = getRootOrStopBeforeParenthesis(tree->appendChild(new Number{token->getValue()}));
    return addImplicitMultiplication(tree, token);
}

Node *parseVariable(Node *tree, Node *token) {
    tree = getRootOrStopBeforeParenthesis(tree->appendChild(token->copy()));
    return addImplicitMultiplication(tree, token);
}

Node *parseOperator(Node *tree, Node *token) {
    if (token->getTokenType() == Token::DoubleTimes) token->setTokenType(Token::Caret);
    Node *nextToken = token->getNext();
    if (nextToken != nullptr) {
        Token nextTokenType = nextToken->getTokenType();
        if (isOperator(nextTokenType)) {
            if (nextTokenType != Token::Minus) {
                throw InvalidExpression(OperatorsString(token->getTokenType()) + OperatorsString(nextToken->getTokenType()));
            }
            if (token->getTokenType() == Token::Minus) nextToken->setTokenType(Token::Plus);
            return tree;
        }
    }

    if ((token->getValue() == "-") && (tree == nullptr || tree->getTokenType() == Token::OpeningParenthesis)) {
        tree = tree->appendChild(new Number{"0"});
    }
    if (!isOperator(tree->getTokenType()) || getOperatorPriority(token->getTokenType()) <= getOperatorPriority(tree->getTokenType())) {
        Node *tempNode = token->copy();
        tempNode->appendChild(tree->copy());
        tree->replaceData(tempNode);
        delete tempNode;
    }
    else {
        Node *child;
        child = getLastChild(tree);
        while (isOperator(child->getTokenType()) && getOperatorPriority(token->getTokenType()) > getOperatorPriority(child->getTokenType()))
            child = getLastChild(child);
        Node *tempNode = token->copy();
        tempNode->appendChild(child->copy());
        child->replaceData(tempNode);
        delete tempNode;
        tree = child;
    }
    return tree;
}
Node *parseOpeningParenthesis(Node *tree, Node *token) {
    return tree->appendChild(token->copy());
}

Node *parseClosingParenthesis(Node *tree, Node *token) {
    tree = getRootOrStopBeforeParenthesis(tree);
    if (isNodeNull(tree->getParent())) {
        throw MissingToken("(");
    }
    tree = tree->getParent();
    tree->setTokenType(Token::ClosingParenthesis);
    if (isNodeNull(tree->getParent())) tree = tree->getParent();
    return addImplicitMultiplication(tree, token);
}

Node *addImplicitMultiplication(Node *tree, Node *token) {
    Node *nextToken = token->getNext();
    if (nextToken  == nullptr) return tree;
    Token nextTokenType = nextToken->getTokenType();
    if (nextTokenType == Token::Number ||
            nextTokenType == Token::Name ||
            nextTokenType == Token::OpeningParenthesis) {
        return parseOperator(tree, new Node{Token::ImplicitTimes});
    }
    return tree;
}