using namespace std;

#include "parser.hpp"
#include "functions.hpp"
#include "addition.hpp"
#include "substraction.hpp"
#include "multiplication.hpp"
#include "division.hpp"
#include "power.hpp"
#include "factorial.hpp"

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
    try {
        while (expressionTokens != nullptr) {
            switch (expressionTokens->getTokenType())
            {
            case Token::Number:
                parseNumber();
                break;
            case Token::Name:
                parseVariable();
                break;
            case Token::Comma:
                parseComma();
                break;
            // should be changed where operators will be complete
            case Token::Plus:
            case Token::Minus:
            case Token::Times:
            case Token::Slash:
            case Token::Caret:
            case Token::DoubleTimes:
                parseOperator();
                break;
            case Token::OpeningParenthesis:
                parseOpeningParenthesis();
                break;
            case Token::ClosingParenthesis:
                parseClosingParenthesis();
                break;
            default:
                throw UnknownToken(*expressionTokens);
                break;
            }
            if (settings->debug) {
                cerr << endl << "Root : " << endl;
                cerr << *expressionTreeRoot << endl;
                root(expressionTree)->display(cerr);
                cerr << endl;
            }
            expressionTokens = expressionTokens->getNext();
        }
        removeParenthesis(expressionTreeRoot);
        replaceImplicitTimes(expressionTreeRoot);
    }
    catch (const exception &) {
        expressionTree = nullptr;
        delete expressionTreeRoot;
        throw;
    }
    if (expressionTree == expressionTreeRoot) {
        expressionTree = nullptr;
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
        throw InvalidExpression(tokenToString(expressionTokens->getTokenType()));
    }
    expressionTree = expressionTree->getParent();
}

void Parser::parseOperator() {
    Node *actualToken = expressionTokens;
    Node *nextToken = actualToken->getNext();
    if (nextToken != nullptr) {
        Token nextTokenType = nextToken->getTokenType();
        if (isOperator(nextTokenType)) {
            // Avoid things like -/ or /*
            // but allow a second operator when it's a substraction
            if (nextTokenType != Token::Minus) {
                string actualTokenTypeString = OperatorsString(actualToken->getTokenType());
                string nextTokenTypeString = OperatorsString(nextToken->getTokenType());
                throw InvalidExpression(actualTokenTypeString + nextTokenTypeString);
            }
            //  Replace -- with +
            if (actualToken->getTokenType() == Token::Minus) {
                expressionTokens = expressionTokens->getNext();
                actualToken->setTokenType(Token::Plus);
            }
        }
    }
    // ** is equivalent to ^, they both mean exponential
    if (actualToken->getTokenType() == Token::DoubleTimes) actualToken->setTokenType(Token::Caret);

    // if there is a substraction and then a value, we help the solver by adding a 0 before the substraction
    // can be great to find a solution to remove it
    else if ((actualToken->getTokenType() == Token::Minus) && (isNodeNull(expressionTree) || expressionTree->getTokenType() == Token::OpeningParenthesis)) {
        expressionTree = expressionTree->appendChild(new Number{"0"});
    }

    Node *newNode;
    switch (actualToken->getTokenType())
    {
    case Token::Plus:
        newNode = new Addition();
        break;
    case Token::Minus:
        newNode = new Substraction();
        break;
    case Token::ImplicitTimes:
        newNode = new Multiplication();
        newNode->setTokenType(Token::ImplicitTimes);
        break;
    case Token::Times:
        newNode = new Multiplication();
        break;
    case Token::Slash:
        newNode = new Division();   
        break;
    case Token::Caret:
        newNode = new Power();
        break;
    case Token::Bang:
        newNode = new Factorial();
        break;
    default:
        throw InvalidExpression("Invalid token " + tokenToString(actualToken->getTokenType())); // should never happen
    }

    // if the actual operator node have the lower priority, directly take the last node and add it as a child of the operator
    if (!isOperator(expressionTree->getTokenType()) || getOperatorPriority(actualToken->getTokenType()) <= getOperatorPriority(expressionTree->getTokenType())) {
        newNode->appendChild(expressionTree->copyNodeWithChilds());
        expressionTree->getParent()->replaceChild(expressionTree, newNode);
        expressionTree = newNode;
    }
    else {
        // else go through the tree to insert the operator inside the others who don't have the priority
        Node *child;
        child = getLastChild(expressionTree);
        while (isOperator(child->getTokenType()) && getOperatorPriority(actualToken->getTokenType()) > getOperatorPriority(child->getTokenType()))
            child = getLastChild(child);
        newNode->appendChild(child->copyNodeWithChilds());
        child->getParent()->replaceChild(child, newNode);
        expressionTree = newNode;
    }
}
void Parser::parseOpeningParenthesis() {
    expressionTree = expressionTree->appendChild(expressionTokens->copyNodeWithChilds());
}

void Parser::parseClosingParenthesis() {
    if (expressionTree->getTokenType() == Token::OpeningParenthesis) {// useless parenthesis or function without argument parenthesis
        if (expressionTree->getParent()->getTokenType() == Token::Function) {
            expressionTree->setTokenType(Token::Empty); // no parameter function is registered with empty child
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
    if (nextToken == nullptr) return;
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