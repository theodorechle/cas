#ifndef TREE_HPP
#define TREE_HPP

#include <iostream>
#include <string>

#include "constants.hpp"

using namespace constants;

class TypeError: public std::exception {
    std::string msg;
public:
    TypeError(const std::string& msg=""): msg{msg} {};
    const char* what() const noexcept override {return msg.c_str();};
};

class Node {
        std::string value;
        constants::Types type;
        Node *parent;
        Node *next = NULL;
        Node *child = NULL;
        Node *copyNodeAndChilds();
        void displayTree(int level) const;
public :
        Node(constants::Types type=constants::Types::NUL, std::string value="", Node *parent=nullptr): type{type}, parent{parent}, value{value} {};

        void setValue(std::string s) {value = s;};
        const std::string& getValue() const {return value;};
        void setType(constants::Types t) {type = t;};
        constants::Types getType() const {return type;};
        Node *getParent() const {return parent;};
        void setParent(Node *parent) {this->parent = parent;};
        Node *getNext() const {return next;};
        void setNext(Node *next) {this->next = next;};
        void removeNext() {setValue(nullptr);};
        Node *getChild() const {return child;};
        /*Set the first child (the one who is directly pointed by the instance)*/
        void setChild(Node *child);
        void removeChild() {setChild(nullptr);};
        /*Add a child at the end of them*/
        Node *appendChild(Node *);
        Node *addEmptyChild();
        /**
            *Replace the data of the node (value, type and childs)
            *by the ones of the given tree.
        */
        void replaceData(Node *tree);
        /**
            *Copy self node and childs, not brothers
        */
        Node *copyNode() const;
        void display() const;
        string *str() const;

        ~Node();
};

/**
    *Return the parent who don't have a parent
*/
Node *root(Node *node);

Node *getLastChild(Node *n);

int getPriority(const string &ope);

#endif // TREE_HPP