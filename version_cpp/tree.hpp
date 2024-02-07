#ifndef TREE_HPP
#define TREE_HPP

#include <iostream>
#include <string>

#include "constants.hpp"

class Node {
    std::string value;
    constants::Types type;
    Node* parent;
    Node* next = NULL;
    Node* child = NULL;

    void displayTree(int level) const;
public :
    Node(constants::Types type=constants::Types::NUL, std::string value="", Node* parent=nullptr): type{type}, parent{parent}, value{value} {};

    void setValue(std::string s) {value = s;};
    const std::string& getValue() const {return value;};
    void setType(constants::Types t) {type = t;};
    constants::Types getType() const {return type;};
    Node* getParent() const {return parent;};
    void setParent(Node* parent) {this->parent = parent;};
    Node* getNext() const {return next;};
    void setNext(Node* next) {this->next = next;};
    void removeNext() {setValue(nullptr);};
    Node* getChild() const {return child;};
    void setChild(Node* child) {this->child = child;};
    void removeChild() {setChild(nullptr);};

    /**
     * Copy self node and childs
    */
    Node* copyNode() const;
    void display() const;
    /**
     * Return the parent who don't have a parent
    */
    Node* root() const;
    char* str() const;
};

Node* getLastChild(Node* n) {
    if (n == nullptr) return n;
    while (n->getChild()) n = n->getChild();
    return n;
}

#endif // TREE_HPP