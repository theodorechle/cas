#include "tree.hpp"

Node *Node::copyNodeAndChilds() {
    Node *n = new Node;
    Node *child = getChild();
    Node *next = getNext();
    n->setType(getType());
    n->setValue(getValue());
    if (child != nullptr) n->setChild(child->copyNodeAndChilds());
    if (next != nullptr) n->setNext(next->copyNodeAndChilds());
    return n;
}

Node *Node::copyNode() const {
    Node *n = new Node;
    Node *child = getChild();
    n->setType(getType());
    n->setValue(getValue());
    if (n != nullptr) n->setChild(child->copyNodeAndChilds());
    return n;
}


void Node::display() const {

}

string *Node::str() const {
    bool parenthesis;
    Node *child = new Node{};
    string *s = new string{};
    if (getType() == Types::OPT) {
        // add parenthesis if father is operator and have bigger priority
        if (getParent() != NULL && getParent()->getType() == Types::OPT && getPriority(getValue()) < getPriority(getParent()->getValue())) {
            parenthesis = true;
            *s += "(";
        }
        *s += *getChild()->str();
        *s += getValue();
        *s += *getChild()->getNext()->str();
        if (parenthesis) *s += ")";
    }
    else if (getType() == Types::FUC)
    {
        *s += getValue();
        *s += "(";
        child = getChild();
        if (child != nullptr) {
            *s += *child->str();
            child = child->getNext();
            while (child != nullptr) {
                *s += ", ";
                *s += *child->str();
                child = child->getNext();
            }
        }
        *s += ")";
    }
    else *s += getValue();
    return s;
}

void Node::setChild(Node *child)  {
    delete getChild();
    this->child = child;
    while (child != NULL) {
        child->setParent(this);
        child = child->getNext();
    }
}

Node* Node::appendChild(Node *child) {
    Node* c = getChild();
    if (c == nullptr) {
        setChild(child);
        return c;
    }
    while (c->getNext() != nullptr) c = c->getNext();
    c->setNext(child);
    child->setParent(this);
    return c;
}

Node *Node::addEmptyChild() {
    Node *child = new Node;
    return appendChild(child);
}

void Node::replaceData(Node *tree) {
    setValue(tree->getValue());
    setType(tree->getType());
    setChild(tree->getChild());
}

Node::~Node() {
    delete parent;
    delete next;
    delete child;
}

Node *root(Node *node) {
    if (node == nullptr || node->getParent() == nullptr) return node;
    return root(node);
}

Node *getLastChild(Node *n) {
    if (n == nullptr) return n;
    while (n->getChild()) n = n->getChild();
    return n;
}

int getPriority(const string &ope) {
    if (ope == ADDITION_SIGN) return ADDITION_PRIORITY;
    if (ope == SUBSTRACTION_SIGN) return SUBSTRACTION_PRIORITY;
    if (ope == MULTIPLICATION_SIGN) return MULTIPLICATION_PRIORITY;
    if (ope == DIVISION_SIGN) return DIVISION_PRIORITY;
    if (ope == POWER_SIGN) return POWER_PRIORITY;
    if (ope == IMPLICIT_MULTIPLICATION_SIGN) return IMPLICIT_MULTIPLICATION_PRIORITY;
    else throw TypeError("Error in priority : '" + ope + "' is not an operator");
}