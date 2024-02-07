#include "parser.hpp"

bool isTypeOrEmpty(int type, int checkType) {
    return (!type || type == checkType);
}

bool isOperator(string value) {
    for (int i=0; i < NB_OPE; i++)
        if (value == OPERATORS[i])
            return true;
    return false;
}

Node* addTreeByValues(Node* t, string* value, Types type) {
    if (type == Types::NUL) {
        throw NullError(*value);
    }
    Node* tree{};
    tree->setValue(value);
    tree->setType(type);
    t->setNext(tree);
    return tree;
}
