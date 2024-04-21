#include <iostream>
using namespace std;

#include "node.hpp"
#include "functions.hpp"

using namespace constants;


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

Node *Node::copy() const {
    Node *n = new Node;
    Node *child = getChild();
    n->setType(getType());
    n->setValue(getValue());
    if (child != nullptr) n->setChild(child->copyNodeAndChilds());
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
    displayTree(flow, 0);
}

void Node::setParent(Node *parent) {
    Node *node = this;
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
        if (getParent() != nullptr && OPERATORS.find(getParent()->getValue()) != OPERATORS.end() && getPriority(getValue()) <= getPriority(getParent()->getValue())) {
            parenthesis = true;
            s += "(";
        }
        if (getChild() != nullptr)
            s += getChild()->str();
        s += getValue();
        if (getChild() != nullptr && getChild()->getNext() != nullptr)
            s += getChild()->getNext()->str();
        if (parenthesis) s += ")";
    }
    else if (getType() == Types::Function)
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
    this->child = child;
}

void Node::setNext(Node *next) {
    delete getNext();
    this->next = next;
}

void Node::appendNext(Node *next) {
    Node *c = this;
    while (c->getNext() != nullptr) {
        c = c->getNext();
    }
    c->setNext(next);
}

Node *Node::appendChild(Node *child) {
    Node *c = getChild();
    if (c == nullptr) setChild(child);
    else c->appendNext(child);
    child->parent = this;
    return child;
}

Node *Node::appendChild(Node &child) {
    Node *c = getChild();
    if (c == nullptr) setChild(&child);
    else c->appendNext(&child);
    child.parent = this;
    return &child;
}

Node *Node::addEmptyChild() {
    Node *child = new Node{};
    return appendChild(child);
}

void Node::replaceData(Node *tree) {
    setValue(tree->getValue());
    setType(tree->getType());
    delete getChild();
    setChild(tree->getChild());
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
    if (node == nullptr ||
        node->getParent() == nullptr ||
        node->getParent()->getType() == Types::NullRoot) return node;
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
    unordered_map<string, int>::const_iterator iter = OPERATORS.find(ope);
    if (iter != OPERATORS.cend()) return iter->second;
    return DEFAULT_PRIORITY;
}

bool areSameNodes(const Node *node1, const Node *node2) {
    if (!(node1 == node2)) return false;
    Node *child1 = node1->getChild();
    Node *child2 = node2->getChild();
    while (child1 != nullptr) {
        if (child2 == nullptr) return false;
        if (!areSameNodes(child1, child2)) return false;
        child1 = child1->getNext();
        child2 = child2->getNext();
    }
    return (child2 == nullptr);
}

void deleteNullRoot(Node *node) {
    node = root(node);
    if (node->getParent() == nullptr) return;
    node->getParent()->setChild(nullptr);
    delete node->getParent();
    node->setParent(nullptr);
}

bool operator==(const Node &n1, const Node &n2) {
    return (n1.getValue() == n2.getValue() || n1.getType() == n2.getType());
}

std::ostream &operator<<(std::ostream &o, const Node &n) {
    o << n.str();
    return o;
}