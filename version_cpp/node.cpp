#include <iostream>
using namespace std;

#include "node.hpp"
#include "functions.hpp"

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

void Node::displayTree(ostream &flow, int level) const {
    for (int i=0; i<level; i++) flow << "\t";
    flow << getValue() << " (" << getType() << ")" << endl;
    Node * child = getChild();
    while (child != nullptr) {
        child->displayTree(flow, level+1);
        child = child->next;
    }
}

void Node::display(ostream &flow) const {
    displayTree(flow);
}

void Node::setParent(Node *parent) {
    Node * node = this;
    while (node != nullptr) {
        node->parent = parent;
        node = node->getNext();
    }
}

string Node::str() const {
    bool parenthesis = false;
    Node *child;
    string s;
    if (OPERATORS.find(getValue()) != OPERATORS.end()) {
        // add parenthesis if father is operator and has bigger priority
        if (getParent() != nullptr && OPERATORS.find(getParent()->getValue()) != OPERATORS.end() && getPriority(getValue()) < getPriority(getParent()->getValue())) {
            parenthesis = true;
            s += "(";
        }
        s += getChild()->str();
        s += getValue();
        s += getChild()->getNext()->str();
        if (parenthesis) s += ")";
    }
    else if (getType() == Types::FUC)
    {
        s += getValue();
        s += "(";
        child = getChild();
        if (child != nullptr) {
            s += child->str();
            child = child->getNext();
            while (child != nullptr) {
                s += ", ";
                s += child->str();
                child = child->getNext();
            }
        }
        s += ")";
    }
    else s += getValue();
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
    if (c == nullptr) setChild(child);
    else c->appendNext(child);
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
    cerr << "tree" << endl;
    tree->display(cerr);
    if (getChild() != nullptr) {
        getChild()->setParent(this);   
        tree->setChild(nullptr);
    }
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
    if (n == nullptr) return nullptr;
    n = n->getChild();
    if (n == nullptr) return nullptr;
    while (n->getNext() != nullptr) n = n->getNext();
    return n;
}

int getPriority(const string &ope) {
    map<string, int>::const_iterator iter = PRIORITIES.find(ope);
    if (iter != PRIORITIES.cend()) return iter->second;
    return DEFAULT_PRIORITY;
}