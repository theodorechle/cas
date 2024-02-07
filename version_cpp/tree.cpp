#include "tree.hpp"

using namespace std;

string* createString() {
    string* s = new string("");
    return s;
}

Node::Node(int type, Node* parent)
: type{type}, parent{parent}
{
    value = createString();
}

string* Node::getValue() {
    return value;
}

int Node::getType() {
    return type;
}

Node* Node::getParent() {
    return parent;
}

Node* Node::setParent(Node* parent) {
    this->parent = parent;
    return parent;
}

Node* Node::getNext() {
    return next;;
}

void Node::setNext(Node* next) {
    this->next = next;
}

void Node::removeNext() {
    setNext(nullptr);
}

Node* Node::getChild() {
    return child;
}

void Node::setChild(Node* child) {
    this->child = child;
}

void Node::removeChild() {
    setChild(nullptr);
}