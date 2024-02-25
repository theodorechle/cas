#include "tree.hpp"

using namespace constants;

Node *Node::copyNodeAndChilds() {
    Node *n;
    Node *child = getChild();
    Node *next = getNext();
    n->setType(getType());
    n->setValue(getValue());
    if (child != nullptr) n->setChild(child->copyNodeAndChilds());
    if (next != nullptr) n->setNext(next->copyNodeAndChilds());
    return n;
}

Node *Node::copyNode() const {
    Node *n;
    Node *child = getChild();
    n->setType(getType());
    n->setValue(getValue());
    if (n != nullptr) n->setChild(child->copyNodeAndChilds());
    return n;
}

void Node::displayTree(int level) const {
    for (int i=0; i<level; i++)
        cout << "\t";
    cout << getValue() << " (" << getType() << ")" << endl;
    Node * child = getChild();
    while (child != nullptr) {
        child->displayTree(level+1);
        child = child->next;
    }
}

void Node::display() const {
    displayTree();
}

void Node::setParent(Node *parent) {
    Node * node = this;
    while (node != nullptr) {
        node->parent = parent;
        node = node->getNext();
    }
}

string *Node::str() const {
    bool parenthesis = false;
    Node *child;
    string *s = new string{""};
    if (getType() == Types::OPT) {
        // add parenthesis if father is operator and has bigger priority
        if (getParent() != nullptr && getParent()->getType() == Types::OPT && getPriority(getValue()) < getPriority(getParent()->getValue())) {
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
}

void Node::setNext(Node *next) {
    delete getNext();
    this->next = next;
}

void Node::appendNext(Node *next) {
    Node *c = this;
    while (c->getNext() != nullptr) c = c->getNext();
    c->setNext(next);
}

Node *Node::appendChild(Node *child) {
    Node *c = getChild();
    if (c == nullptr) {
        setChild(child);
        return child;
    }
    c->appendNext(child);
    child->parent = this;
    return child;
}

Node *Node::addEmptyChild() {
    Node *child = new Node{};
    return appendChild(child);
}

void Node::replaceData(Node *tree) {
    setValue(tree->getValue());
    setType(tree->getType());
    setChild(tree->getChild());
    getChild()->setParent(this);
    tree->child = nullptr;
}

Node::~Node() {
    if (next != nullptr) delete next;
    if (child != nullptr) delete child;
}

Node *root(Node *node) {
    if (node == nullptr || node->getParent() == nullptr) return node;
    return root(node->getParent());
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