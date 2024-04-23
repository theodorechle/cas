using namespace std;

#include "parser.hpp"
#include "functions.hpp"
#include "constants.hpp"
#include "number.hpp"

using namespace constants;

const char* UnknownToken::what() const noexcept {
      string *str =  new string("Error : Unknown token '" + token.getValue() + " (" + TokensStr(token.getType()) + ")'");
      return str->c_str();
}

const char* MissingToken::what() const noexcept {
      string *str =  new string("Error : Missing token '" + token + "'");
      return str->c_str();
}

void removeParenthesis(Node *t) {
    if (t->getType() == Types::ClosingParenthesis) t->replaceData(t->getChild());
    else if (t->getType() == Types::OpeningParenthesis) throw MissingToken(")");
    Node *child = t->getChild();
    while (child != nullptr) {
        removeParenthesis(child);
        child = child->getNext();
    }
}

void changeImplicitToExplicitMultiplications(Node *t) {
    if (t->getType() == Types::Operator && t->getValue() == IMPLICIT_MULTIPLICATION_SIGN) t->setValue(MULTIPLICATION_SIGN);
    Node *child = t->getChild();
    while (child != nullptr) {
        changeImplicitToExplicitMultiplications(child);
        child = child->getNext();
    }
}

Node *getRootOrParenthesis(Node *tree) {
    if (tree == nullptr ||
        tree->getType() == Types::OpeningParenthesis ||
        tree->getParent() == nullptr ||
        tree->getParent()->getType() == Types::NullRoot) return tree;
    return getRootOrParenthesis(tree->getParent());
}

Node *parser(Node *tokenList, bool debug, bool implicitPriority) {
    Node *tree = new Node{"", Types::NullRoot};
    if (tokenList == nullptr) return tree;
    while (tokenList != nullptr) {
        switch (tokenList->getType())
        {
        case Types::Number:
            tree = parseNumber(tree, tokenList);
            break;
        case Types::Variable:
            tree = parseVariable(tree, tokenList);
            break;
        case Types::Operator:
            tree = parseOperator(tree, tokenList);
            break;
        case Types::OpeningParenthesis:
            tree = parseOpeningParenthesis(tree, tokenList);
            break;
        case Types::ClosingParenthesis:
            tree = parseClosingParenthesis(tree, tokenList);
            break;
        default:
            throw UnknownToken(*tokenList);
            break;
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
    // changeImplicitToExplicitMultiplications(tree);
    deleteNullRoot(tree);
    return tree;
}

Node *parseNumber(Node *tree, Node *token) {
    return getRootOrParenthesis(tree->appendChild(new Number{token->getValue()}));
}

Node *parseVariable(Node *tree, Node *token) {
    return getRootOrParenthesis(tree->appendChild(token->copy()));
}

Node *parseOperator(Node *tree, Node *token) {
    if ((token->getValue() == SUBSTRACTION_SIGN) && (tree == nullptr || tree->getType() == Types::OpeningParenthesis)) {
        tree = tree->appendChild(new Number{"0"});
    }
    if (tree->getType() != Types::Operator || getPriority(token->getValue()) <= getPriority(tree->getValue())) {
        Node *tempNode = token->copy();
        tempNode->appendChild(tree->copy());
        tree->replaceData(tempNode);
        delete tempNode;
    }
    else {
        Node *child;
        child = getLastChild(tree);
        while (child->getType() == Types::Operator && getPriority(token->getValue()) > getPriority(child->getValue()))
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
    tree = getRootOrParenthesis(tree);
    if (tree->getType() != Types::OpeningParenthesis) {
        throw MissingToken("(");
    }
    tree->setType(Types::ClosingParenthesis);
    if (tree->getParent() != nullptr && tree->getParent()->getType() != Types::NullRoot) tree = tree->getParent();
    return tree;
}