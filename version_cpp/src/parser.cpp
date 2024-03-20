using namespace std;

#include "parser.hpp"
#include "functions.hpp"

using namespace constants;

void removeParenthesis(Node *t) {
    if (t->getType() == Types::CPA) t->replaceData(t->getChild());
    Node *child = t->getChild();
    while (child != nullptr) {
        removeParenthesis(child);
        child = child->getNext();
    }
}

Node *findRootOrParenthesis(Node *tree) {
    if (tree->getParent() == nullptr || tree->getParent()->getType() == Types::OPA) return tree;
    return findRootOrParenthesis(tree->getParent());
}

Node *parser(Node *tokenList, bool debug, bool implicitPriority) {
    Node *tree = new Node{};
    Types treeType = Types::NUL;
    if (tokenList == nullptr) return tree;
    while (tokenList != nullptr) {
        treeType = tokenList->getType();
        if (treeType == Types::NBR || treeType == Types::VAR) {
            tree->replaceData(tokenList);
            tree = findRootOrParenthesis(tree);
        }
        else if (treeType == Types::OPT) {
            if ((tokenList->getValue() == SUBSTRACTION_SIGN) && (tree == nullptr || treeType == Types::OPA)) {
                tree->replaceData(new Node{Types::NBR, "0"});
            }
            if (tokenList->getValue() == IMPLICIT_MULTIPLICATION_SIGN) tokenList->setValue(MULTIPLICATION_SIGN);
            if (tree->getType() != Types::OPT || getPriority(tokenList->getValue()) <= getPriority(tree->getValue())) {
                tokenList->addEmptyChild()->replaceData(tree);
                tree->replaceData(tokenList);
                tree = tree->addEmptyChild();
            }
            else {
                Node *child;
                child = getLastChild(tree);
                while (child->getType() == Types::OPT && getPriority(tokenList->getValue()) > getPriority(child->getValue()))
                    child = getLastChild(child);
                tokenList->addEmptyChild()->replaceData(child);
                child->replaceData(tokenList);
                tree = child->addEmptyChild();
            }
        }
        else if (treeType == Types::OPA) {
            if (tree->getType() != Types::NUL) tree = tree->addEmptyChild();
            tree->replaceData(tokenList);
            tree = tree->addEmptyChild();
        }
        else if (treeType == Types::CPA) {
            tree = findRootOrParenthesis(tree);
            if (tree->getParent() != nullptr) {
                tree = tree->getParent();
                tree->setType(Types::CPA);
            }
            if (tree->getParent() != nullptr) tree = tree->getParent();
        }
        if (debug) {
            cerr << endl << "Root : " << endl;
            root(tree)->display(cerr);
            cerr << endl;
        }
        tokenList = tokenList->getNext();
    }
    removeParenthesis(tree);
    return root(tree);
}
