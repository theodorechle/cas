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

Node *addTreeByValues(Node *t, string *value, Types type) {
    if (type == Types::NUL) {
        throw NullError(*value);
    }
    Node *tree = new Node;
    tree->setValue(*value);
    tree->setType(type);
    t->setNext(tree);
    return tree;
}

Node *parser(string expr, bool debug, bool implicitPriority) {
    Node *exprList = new Node;
    string *value = new string;
    string *testString = new string;
    bool createNewTree;
    char character;
    int len = expr.length(), index = 0;
    Types type = Types::NUL;
    while (index < len) {
        createNewTree = true;
        character = expr[index];
        if (debug) cout << "character : " << character << endl;
        if (isspace(character)) {
            if (debug) cout << "Found type of character '" << character << "' is space" << endl;
            if (!value)
                createNewTree = false;
        }
        else if (isalpha(character)) {
            if (debug) cout << "Found type of character '" << character << "' is alpha" << endl;
            if (isTypeOrEmpty(type, Types::VAR)) {
                *value += character;
                type = Types::VAR;
                createNewTree = false;
            }
            else {
                if (type == Types::NBR || type == Types::CPA) {
                    exprList = addTreeByValues(exprList, value, type);
                    value->clear();
                    if (implicitPriority) *value += IMPLICIT_MULTIPLICATION_SIGN;
                    else *value += MULTIPLICATION_SIGN;
                    type = Types::OPT;
                }
                index--;
            }
        }
        else if (character == OPENING_PARENTHESIS_SIGN[0]) {
            if (debug) cout << "Found type of character '" << character << "' is opening parenthesis" << endl;
            if (type == Types::NUL) type = Types::OPA;
            else {
                if (type == Types::NBR || type == Types::VAR) {
                    exprList = addTreeByValues(exprList, value, type);
                    value->clear();
                    if (implicitPriority) *value += IMPLICIT_MULTIPLICATION_SIGN;
                    else *value += MULTIPLICATION_SIGN;
                    type = Types::OPT;
                }
                index--;
            }
        }
        else if (character == CLOSING_PARENTHESIS_SIGN[0]) {
            if (debug) cout << "Found type of character '" << character << "' is closing parenthesis" << endl;
            if (type == Types::NUL) type = Types::CPA;
            else index --;
        }
        else if (isdigit(character) || character == '.') {
            if (debug) cout << "Found type of character '" << character << "' is number" << endl;
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
                    if (debug) cout << "Found type of value '" << *value << "' is operator" << endl;
                }
                else
                    if (debug) cout << "Type of character '" << character << "' not found" << endl;
                createNewTree = false;
                testString->clear();
            }
        if (debug) cout << "value : " << *value << " type : " << type << endl;
        if (createNewTree) {
            exprList = addTreeByValues(exprList, value, type);
            value->clear();
            type = Types::NUL;
        }
        index++;
    }
    if (value)
        exprList = addTreeByValues(exprList, value, type);
    return exprList;
}

Node *findRootOrParenthesis(Node *tree) {
    if (tree->getParent() == nullptr || tree->getParent()->getType() == Types::OPA) return tree;
    return findRootOrParenthesis(tree->getParent());
}

Node *parsedToTree(Node *exprList, bool debug, bool implicitPriority) {
    Node *tree = new Node;
    Node *t = new Node;
    Node *child = new Node;
    Node *tmpNode = new Node;
    Types ttype = Types::NUL;
    if (exprList == nullptr || exprList == nullptr) return tree;
    while (exprList != nullptr) {
        ttype = exprList->getType();
        if (ttype == Types::NBR || ttype == Types::VAR) {
            tree->replaceData(exprList);
            tree = findRootOrParenthesis(tree);
        }
        else if (ttype == Types::OPT) {
            if ((exprList->getValue() == SUBSTRACTION_SIGN) && (tree == nullptr || tree->getType() == Types::OPA))
                tmpNode->setValue("0");
                tmpNode->setType(Types::NBR);
                tree->replaceData(tmpNode);
            if (tree->getType() != Types::OPT || getPriority(exprList->getValue()) <= getPriority(tree->getValue())) {
                exprList->addEmptyChild()->replaceData(tree);
                tree->replaceData(t);
                tree = tree->addEmptyChild();
            }
            else {
                child = getLastChild(tree);
                while (child->getType() == Types::OPT && getPriority(t->getValue()) > getPriority(child->getValue()))
                    child = getLastChild(child);
                if (exprList->getValue() == IMPLICIT_MULTIPLICATION_SIGN) exprList->setValue(MULTIPLICATION_SIGN);
                exprList->addEmptyChild()->replaceData(child);
                tree = getLastChild(child->getParent()->addEmptyChild());
                tree->replaceData(exprList);
            }
        }
        else if (ttype == Types::OPA) {
            if (tree->getType() != Types::NUL) tree = tree->addEmptyChild();
            tree->replaceData(exprList);
            tree = tree->addEmptyChild();
        }
        else if (ttype == Types::CPA) {
            tree->getParent()->replaceData(tree);
            tree = findRootOrParenthesis(tree);
        }
        if (debug) {
            printf("\nRoot : \n");
            root(tree)->display();
            printf("\n");
        }
        exprList = exprList->getNext();
    }
    return root(tree);
}
