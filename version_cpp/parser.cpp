#include "parser.hpp"

bool isTypeOrEmpty(Types type, Types checkType) {
    return (type == Types::NUL || type == checkType);
}

bool isOperator(string *value) {
    for (int i=0; i < NB_OPE; i++)
        if (*value == OPERATORS[i])
            return true;
    return false;
}

void addTreeByValues(Node &t, string *value, Types type) {
    if (type == Types::NUL) {
        throw NullError(*value);
    }
    if (t.getType() == Types::NUL) {
        t.setValue(*value);
        t.setType(type);
        return;
    }
    Node *tree = new Node{};
    tree->setValue(*value);
    tree->setType(type);
    t.appendNext(tree);
}

Node *parser(string &expr, bool debug, bool implicitPriority) {
    Node *exprList = new Node{};
    string *value = new string{""};
    string *testString = new string{""};
    bool createNewTree;
    char character;
    int len = expr.length(), index = 0;
    Types type = Types::NUL;
    while (index < len) {
        createNewTree = true;
        character = expr[index];
        if (debug) cerr << "character : " << character << endl;
        if (isspace(character)) {
            if (debug) cerr << "Found type of character '" << character << "' is space" << endl;
            if (!value)
                createNewTree = false;
        }
        else if (isalpha(character)) {
            if (debug) cerr << "Found type of character '" << character << "' is alpha" << endl;
            if (isTypeOrEmpty(type, Types::VAR)) {
                *value += character;
                type = Types::VAR;
                createNewTree = false;
            }
            else {
                if (type == Types::NBR || type == Types::CPA) {
                    addTreeByValues(*exprList, value, type);
                    value->clear();
                    if (implicitPriority) *value += IMPLICIT_MULTIPLICATION_SIGN;
                    else *value += MULTIPLICATION_SIGN;
                    type = Types::OPT;
                }
                index--;
            }
        }
        else if (character == OPENING_PARENTHESIS_SIGN[0]) {
            if (debug) cerr << "Found type of character '" << character << "' is opening parenthesis" << endl;
            if (type == Types::NUL) type = Types::OPA;
            else {
                if (type == Types::NBR || type == Types::VAR) {
                    addTreeByValues(*exprList, value, type);
                    value->clear();
                    if (implicitPriority) *value += IMPLICIT_MULTIPLICATION_SIGN;
                    else *value += MULTIPLICATION_SIGN;
                    type = Types::OPT;
                }
                index--;
            }
        }
        else if (character == CLOSING_PARENTHESIS_SIGN[0]) {
            if (debug) cerr << "Found type of character '" << character << "' is closing parenthesis" << endl;
            if (type == Types::NUL) type = Types::CPA;
            else index --;
        }
        else if (isdigit(character) || character == '.') {
            if (debug) cerr << "Found type of character '" << character << "' is number" << endl;
            if (isTypeOrEmpty(type, Types::NBR)) {
                *value += character;
                type = Types::NBR;
                createNewTree = false;
            }
            else if (type == Types::VAR) {
                *value += character;
                createNewTree = false;
            }
            else index --;
        }
        else
            if (type != Types::NUL) index--;
            else {
                *testString += *value;
                *testString += character;
                if (isOperator(testString)) {
                    type = Types::OPT;
                    value->clear();
                    *value += *testString;
                    if (debug) cerr << "Found type of value '" << *value << "' is operator" << endl;
                }
                else
                    if (debug) cerr << "Type of character '" << character << "' not found" << endl;
                createNewTree = false;
                testString->clear();
            }
        if (debug) cerr << "value : " << *value << " type : " << type << endl;
        if (createNewTree) {
            addTreeByValues(*exprList, value, type);
            value->clear();
            type = Types::NUL;
        }
        index++;
    }
    if (!value->empty()) addTreeByValues(*exprList, value, type);
    return exprList;
}

Node *findRootOrParenthesis(Node *tree) {
    if (tree->getParent() == nullptr || tree->getParent()->getType() == Types::OPA) return tree;
    return findRootOrParenthesis(tree->getParent());
}

Node *parsedToTree(Node *exprList, bool debug, bool implicitPriority) {
    Node *tree = new Node{};
    Types treeType = Types::NUL;
    if (exprList == nullptr) return tree;
    while (exprList != nullptr) {
        treeType = exprList->getType();
        if (treeType == Types::NBR || treeType == Types::VAR) {
            tree->replaceData(exprList);
            tree = findRootOrParenthesis(tree);
        }
        else if (treeType == Types::OPT) {
            if ((exprList->getValue() == SUBSTRACTION_SIGN) && (tree == nullptr || treeType == Types::OPA)) {
                tree->replaceData(new Node{Types::NBR, "0"});
            }
            if (tree->getType() != Types::OPT || getPriority(exprList->getValue()) <= getPriority(tree->getValue())) {
                exprList->addEmptyChild()->replaceData(tree);
                tree->replaceData(exprList);
                tree = tree->addEmptyChild();
            }
            else {
                Node *child;
                child = getLastChild(tree);
                while (child->getType() == Types::OPT && getPriority(exprList->getValue()) > getPriority(child->getValue()))
                    child = getLastChild(child);
                if (exprList->getValue() == IMPLICIT_MULTIPLICATION_SIGN) exprList->setValue(MULTIPLICATION_SIGN);
                exprList->addEmptyChild()->replaceData(child);
                tree = child;
                tree->replaceData(exprList);
                tree = tree->addEmptyChild();
            }
        }
        else if (treeType == Types::OPA) {
            if (tree->getType() != Types::NUL) tree = tree->addEmptyChild();
            tree->replaceData(exprList);
            tree = tree->addEmptyChild();
        }
        else if (treeType == Types::CPA) {
            tree->getParent()->replaceData(tree);
            tree = findRootOrParenthesis(tree);
        }
        if (debug) {
            tree->display(cerr);
            cerr << endl << "Root : " << endl;
            root(tree)->display(cerr);
            cerr << endl;
        }
        exprList = exprList->getNext();
    }
    return root(tree);
}
