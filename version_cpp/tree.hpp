#ifndef TREE_HPP
#define TREE_HPP

#include <iostream>
#include <string>

class Node {
    std::string value;
    int type;
    Node* parent;
    Node* next = NULL;
    Node* child = NULL;

    void displayTree(int level);
public :
    Node(int type=0, Node* parent=nullptr);

    std::string getValue();
    int getType();
    Node* getParent();
    Node* setParent(Node* parent);
    Node* getNext();
    void setNext(Node* next);
    void removeNext();
    Node* getChild();
    void setChild(Node* child);
    void removeChild();

    /**
     * Copy self node and childs
    */
    Node* copyNode();
    void display();
    /**
     * Return the parent who don't have a parent
    */
    Node* root();
    char* str();
};

char* createString();

#endif // TREE_HPP