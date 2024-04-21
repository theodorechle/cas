using namespace std;

#include "parser.hpp"
#include "functions.hpp"
#include "constants.hpp"
#include "number.hpp"

using namespace constants;

const char* UnknownToken::what() const noexcept {
      string *str =  new string("Error : Unknown token '" + token.getValue() + " (" + TypesStr(token.getType()) + ")'");
      return str->c_str();
}

void removeParenthesis(Node *t) {
    if (t->getType() == Types::ClosingParenthesis) t->replaceData(t->getChild());
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

Node *findRootOrParenthesis(Node *tree) {
    if (tree == nullptr ||
        tree->getParent() == nullptr ||
        tree->getParent()->getType() == Types::OpeningParenthesis ||
        tree->getParent()->getType() == Types::NullRoot) return tree;
    return findRootOrParenthesis(tree->getParent());
}

Node *parser(Node *tokenList, bool debug, bool implicitPriority) {
    Node *tree = new Node{Types::NullRoot};
    Types tokenType;
    if (tokenList == nullptr) return tree;
    while (tokenList != nullptr) {
        tokenType = tokenList->getType();
        if (tokenType == Types::Number) {
            tree = tree->appendChild(new Number{tokenList->getValue()});
            tree = findRootOrParenthesis(tree);
        }
        else if (tokenType == Types::Variable) {
            tree = tree->appendChild(tokenList->copy());
            tree = findRootOrParenthesis(tree);
        }
        else if (tokenType == Types::Operator) {
            if ((tokenList->getValue() == SUBSTRACTION_SIGN) && (tree == nullptr || tree->getType() == Types::OpeningParenthesis)) {
                tree = tree->appendChild(new Number{"0.0"});
            }
            
            // Duplicate code with next block
            if (tree->getType() != Types::Operator || getPriority(tokenList->getValue()) <= getPriority(tree->getValue())) {
                Node *tempNode = tokenList->copy();
                tempNode->appendChild(tree->copy());
                tree->replaceData(tempNode);
                delete tempNode;
            }
            else {
                Node *child;
                child = getLastChild(tree);
                while (child->getType() == Types::Operator && getPriority(tokenList->getValue()) > getPriority(child->getValue()))
                    child = getLastChild(child);
                Node *tempNode = tokenList->copy();
                tempNode->appendChild(child->copy());
                child->replaceData(tempNode);
                delete tempNode;
                tree = child;
            }
        }
        else if (tokenType == Types::OpeningParenthesis) {
            tree = tree->appendChild(tokenList->copy());
        }
        else if (tokenType == Types::ClosingParenthesis) {
            tree = findRootOrParenthesis(tree);
            if (tree->getParent() != nullptr) {
                tree = tree->getParent();
                tree->setType(Types::ClosingParenthesis);
            }
            if (tree->getParent() != nullptr) tree = tree->getParent();
        }
        else {
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
    changeImplicitToExplicitMultiplications(tree);
    deleteNullRoot(tree);
    return tree;
}
