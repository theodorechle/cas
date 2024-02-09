#include "tree.hpp"

Node* Node::copyNode() const {

}


void Node::display() const {

}

Node* root(Node* node) {
    if (node->getParent() == nullptr) return node;
    return root(node->getParent());
}

string* Node::str() const {

}

void Node::setChild(Node* child)  {
    delete getChild();
    this->child = child;
    while (child != NULL) {
        child->setParent(this);
        child = child->getNext();
    }
}

void Node::replaceData(Node* tree) {
    setValue(tree->getValue());
    setType(tree->getType());
    setChild(tree->getChild());
}

Node* getLastChild(Node* n) {
    if (n == nullptr) return n;
    while (n->getChild()) n = n->getChild();
    return n;
}

int getPriority(string operator) {
    switch (operator) {
        case ADDITION_SIGN: return ADDITION_PRIORITY;
        case SUBSTRACTION_SIGN: return SUBSTRACTION_PRIORITY;
        case MULTIPLICATION_SIGN: return MULTIPLICATION_PRIORITY;
        case DIVISION_SIGN: return DIVISION_PRIORITY;
        case POWER_SIGN: return POWER_PRIORITY;
        case IMPLICIT_MULTIPLICATION_SIGN: return IMPLICIT_MULTIPLICATION_PRIORITY;
        default: fprintf(stderr, "Error in priority : '%s' is not an operator\n", operator);
    }
}