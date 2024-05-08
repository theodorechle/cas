using namespace std;

#include "parser.hpp"
#include "functions.hpp"

void Parser::getRootOrStopBeforeParenthesis() {
    if (isNodeNull(expressionTree) ||
            isNodeNull(expressionTree->getParent()) ||
            expressionTree->getParent()->getTokenType() == Token::OpeningParenthesis)
        return;
    expressionTree = expressionTree->getParent();
    getRootOrStopBeforeParenthesis();
}

void Parser::removeParenthesis(Node *t) {
    if (t == nullptr) return;
    if (t->getTokenType() == Token::ClosingParenthesis) {
        if (isNodeNull(t->getChild())) return;
        t->replaceData(t->getChild());
    }
    else if (t->getTokenType() == Token::OpeningParenthesis) throw MissingToken(")");
    Node *child = t->getChild();
    while (child != nullptr) {
        removeParenthesis(child);
        child = child->getNext();
    }
}

void Parser::replaceImplicitTimes(Node *t) {
    if (t == nullptr) return;
    if (t->getTokenType() == Token::ImplicitTimes) t->setTokenType(Token::Times);
    Node *child = t->getChild();
    while (child != nullptr) {
        replaceImplicitTimes(child);
        child = child->getNext();
    }
}

bool Parser::isNodeNull(Node *node) {
    return (node == nullptr || node->getTokenType() == Token::NullRoot);
}

void Parser::parse() {
    while (expressionTokens != nullptr) {
        Token tokenType = expressionTokens->getTokenType();
        if (tokenType == Token::Number) {
            parseNumber();
        }
        else if (tokenType == Token::Name) {
            parseVariable();
        }
        else if (tokenType == Token::Comma) {
            parseComma();
        }
        else if (tokenType == Token::Plus ||
                tokenType == Token::Minus ||
                tokenType == Token::Times ||
                tokenType == Token::Slash ||
                tokenType == Token::Caret ||
                tokenType == Token::DoubleTimes) {
            parseOperator();
        }
        else if (tokenType == Token::OpeningParenthesis) {
            parseOpeningParenthesis();
        }
        else if (tokenType == Token::ClosingParenthesis) {
            parseClosingParenthesis();
        }
        else {
            delete expressionTreeRoot;
            throw UnknownToken(*expressionTokens);
        }
        if (settings->debug) {
            cerr << endl << "Root : " << endl;
            cout << *expressionTreeRoot << endl;
            root(expressionTree)->display(cerr);
            cerr << endl;
        }
        expressionTokens = expressionTokens->getNext();
    }
    removeParenthesis(expressionTreeRoot);
    replaceImplicitTimes(expressionTreeRoot);
    if (expressionTree == expressionTreeRoot) {
        delete expressionTreeRoot;
        expressionTreeRoot = nullptr;
        return;
    }
    expressionTreeRoot = expressionTreeRoot->getChild();
    deleteNullRoot(expressionTreeRoot);
}

void Parser::parseNumber() {
    expressionTree = expressionTree->appendChild(new Number{expressionTokens->getValue()});
    getRootOrStopBeforeParenthesis();
    return addImplicitMultiplication();
}

void Parser::parseVariable() {
    if ( !isFunction(expressionTokens->getValue()) || isNodeNull(expressionTokens->getNext()) || expressionTokens->getNext()->getTokenType() != Token::OpeningParenthesis) {
        expressionTree = expressionTree->appendChild(new Node{Token::Variable, expressionTokens->getValue()});
        getRootOrStopBeforeParenthesis();
        addImplicitMultiplication();
    }
    else {
        expressionTree = expressionTree->appendChild(new Node{Token::Function, expressionTokens->getValue()});
    }
}

void Parser::parseComma() {
    if (isNodeNull(expressionTree->getParent()) || expressionTree->getParent()->getTokenType() != Token::OpeningParenthesis) {
        throw InvalidExpression(tokenToStr(expressionTokens->getTokenType()));
    }
    expressionTree = expressionTree->getParent();
}

void Parser::parseOperator() {
    Node *actualToken = expressionTokens;
    Node *nextToken = actualToken->getNext();
    if (nextToken != nullptr) {
        Token nextTokenType = nextToken->getTokenType();
        if (isOperator(nextTokenType)) {
            if (nextTokenType != Token::Minus) {
                // Avoid things like -/
                string actualTokenTypeString = OperatorsString(actualToken->getTokenType());
                string nextTokenTypeString = OperatorsString(nextToken->getTokenType());
                actualToken = nullptr;
                nextToken = nullptr;
                throw InvalidExpression(actualTokenTypeString + nextTokenTypeString);
            }
            // Avoid modifying expressionTokens
            if (actualToken->getTokenType() == Token::Minus) {
                expressionTokens = expressionTokens->getNext();
                actualToken->setTokenType(Token::Plus);
            }
        }
    }

    if ((actualToken->getTokenType() == Token::Minus) && (isNodeNull(expressionTree) || expressionTree->getTokenType() == Token::OpeningParenthesis)) {
        expressionTree = expressionTree->appendChild(new Number{"0"});
    }
    if (!isOperator(expressionTree->getTokenType()) || getOperatorPriority(actualToken->getTokenType()) <= getOperatorPriority(expressionTree->getTokenType())) {
        Node *tempNode = actualToken->copyNodeWithChilds();
        tempNode->appendChild(expressionTree->copyNodeWithChilds());
        expressionTree->replaceData(tempNode);
        delete tempNode;
    }
    else {
        Node *child;
        child = getLastChild(expressionTree);
        while (isOperator(child->getTokenType()) && getOperatorPriority(actualToken->getTokenType()) > getOperatorPriority(child->getTokenType()))
            child = getLastChild(child);
        Node *tempNode = actualToken->copyNodeWithChilds();
        tempNode->appendChild(child->copyNodeWithChilds());
        child->replaceData(tempNode);
        delete tempNode;
        expressionTree = child;
    }
    if (expressionTree->getTokenType() == Token::DoubleTimes) expressionTree->setTokenType(Token::Caret);
}
void Parser::parseOpeningParenthesis() {
    expressionTree = expressionTree->appendChild(expressionTokens->copyNodeWithChilds());
}

void Parser::parseClosingParenthesis() {
    if (expressionTree->getTokenType() == Token::OpeningParenthesis) {// useless parenthesis or function without argument parenthesis
        if (expressionTree->getParent()->getTokenType() == Token::Function) {
            expressionTree->setTokenType(Token::ClosingParenthesis);
            return;
        }
        throw InvalidExpression("()");
    }
    getRootOrStopBeforeParenthesis();
    if (isNodeNull(expressionTree->getParent())) {
        throw MissingToken("(");
    }
    expressionTree = expressionTree->getParent();
    expressionTree->setTokenType(Token::ClosingParenthesis);
    if (!isNodeNull(expressionTree->getParent())) expressionTree = expressionTree->getParent();
    addImplicitMultiplication();
}

void Parser::addImplicitMultiplication() {
    Node *nextToken = expressionTokens->getNext();
    if (nextToken  == nullptr) return;
    Token nextTokenType = nextToken->getTokenType();
    if (nextTokenType == Token::Number ||
            nextTokenType == Token::Name ||
            nextTokenType == Token::OpeningParenthesis) {
        Node *tempTokens = expressionTokens;
        expressionTokens = new Node{Token::ImplicitTimes};
        parseOperator();
        delete expressionTokens;
        expressionTokens = tempTokens;
    }
}