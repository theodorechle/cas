#include <iostream>

#include "node.hpp"
#include "operator.hpp"
#include "parser.hpp"

using namespace std;

Node *Node::copyNode() const {
    Node *n = createNewNode();
    n->setTokenType(getTokenType());
    n->setValue(getValue());
    return n;
}

/**
 * Copy the node and his childs (not the nexts)
*/
Node *Node::copyNodeWithChilds() const {
    Node *n = copyNode();
    Node *child = getChild();
    if (child != nullptr) n->setChild(child->copyNodeWithChildsAndNexts());
    return n;
}

Node *Node::copyNodeWithChildsAndNexts() const {
    Node *n = copyNodeWithChilds();
    Node *next = getNext();
    if (next != nullptr) n->setNext(next->copyNodeWithChildsAndNexts());
    return n;
}

void Node::displayTree(ostream &flow, int level) const {
    for (int i=0; i<level; i++) flow << "\t";
    flow << getValue() << " (" << getTokenType() << ")" << endl;
    const Node * child = getChild();
    while (child != nullptr) {
        child->displayTree(flow, level+1);
        child = child->next;
    }
}

void Node::display(ostream &flow) const {
    displayTree(flow, 0);
}

void Node::displayNext(ostream &flow) const {
    const Node *next = this;
    while (next != nullptr) {
        flow << next->getValue() << " (" << next->getTokenType() << ")" << endl;
        next = next->getNext();
    }
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
    if (isOperator(getTokenType())) {
        // add parenthesis if father is operator and has bigger priority
        if (getParent() != nullptr) {
        }
        if (getParent() != nullptr &&
        isOperator(getParent()->getTokenType()) &&
        getOperatorPriority(getTokenType()) < getOperatorPriority(getParent()->getTokenType())) {
            parenthesis = true;
            s += "(";
        }
        // check to allow using the method even if the tree is not fully created
        if (getChild() != nullptr)
            s += getChild()->str();
        s += OperatorsString(getTokenType());
        // same as first check
        if (getChild() != nullptr &&
        getChild()->getNext() != nullptr)
            s += getChild()->getNext()->str();
        if (parenthesis) s += ")";
    }
    else if (getTokenType() == Token::Function)
    {
        s += getValue();
        s += "(";
        child = getChild();
        if (child != nullptr && child->getTokenType() != Token::Empty) { // don't print anything in the parenthesis if the function doesn't have parameters
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
    else if (getTokenType() == Token::OpeningParenthesis ||
            getTokenType() == Token::ClosingParenthesis) {
        s += getChild()->str();
    }
    else s += getValue();
    return s;
}

string Node::strNexts() const {
    string s;
    const Node *next = this;
    while (next != nullptr) {
        s += next->getValue();
        next = next->getNext();
    }
    return s;
}

void Node::setChild(Node *child)  {
    this->child = child;
    if (child != nullptr) child->setParent(this);
}

void Node::setNext(Node *next) {
    this->next = next;
}

void Node::removeSpecificChild(Node *child) {
    if (child == nullptr) return;
    if (child == getChild()) {
        setChild(child->getNext());
        child->setNext(nullptr);
        delete child;
        return;
    }
    next = getChild();
    while (next != nullptr)
    {
        if (next->getNext() == child) {
            next->setNext(child->getNext());
            child->setNext(nullptr);
            delete child;
            return;
        }
        next = next->getNext();
    }
    
}

void Node::appendNext(Node *next) {
    Node *c = this;
    while (c->getNext() != nullptr) {
        c = c->getNext();
    }
    c->setNext(next);
    next->setParent(getParent());
}

Node *Node::appendChild(Node *child) {
    Node *c = getChild();
    if (c == nullptr) setChild(child);
    else c->appendNext(child);
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
    if (tree == nullptr) return;
    tree = tree->copyNodeWithChildsAndNexts();
    setValue(tree->getValue());
    setTokenType(tree->getTokenType());
    delete getChild();
    setChild(tree->getChild());
    if (getChild() != nullptr) {
        getChild()->setParent(this);   
        tree->setChild(nullptr);
    }
    delete getNext();
    setNext(tree->getNext());
    tree->setNext(nullptr);
    delete tree;
}

/**
 * Find the child in the childs of the node and replace it with the new one
*/
void Node::replaceChild(Node *child, Node *newChild) {
    if (child == nullptr || newChild == nullptr) return;
    Node *c = getChild();
    if (c == child) {
        newChild->setNext(c->getNext());
        setChild(newChild);
        c->setNext(nullptr);
        delete c;
        return;
    }
    while (c != nullptr) {
        if (c->getNext() == child) {
            delete newChild->getNext();
            newChild->setNext(c->getNext()->getNext());
            c->setNext(newChild);
            newChild->setParent(this);
            child->setNext(nullptr);
            delete child;
            return;
        }
        c = c->getNext();
    }
}

Node::~Node() {
    if (next != nullptr) delete next;
    if (child != nullptr) delete child;
}

Node *root(Node *node) {
    if (Parser::isNodeNull(node) ||
        Parser::isNodeNull(node->getParent())) return node;
    return root(node->getParent());
}

Node *getLastChild(Node *n) {
    if (n == nullptr) return nullptr;
    n = n->getChild();
    if (n == nullptr) return nullptr;
    while (n->getNext() != nullptr) n = n->getNext();
    return n;
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
    return (n1.getValue() == n2.getValue() && n1.getTokenType() == n2.getTokenType());
}

std::ostream &operator<<(std::ostream &o, const Node &n) {
    o << n.str();
    return o;
}

bool isOperator(const Token &token) {
    return (token == Token::Plus ||
    token == Token::Minus ||
    token == Token::Times ||
    token == Token::Slash ||
    token == Token::Caret ||
    token == Token::DoubleTimes ||
    token == Token::ImplicitTimes);
}