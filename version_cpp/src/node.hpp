#ifndef TREE_HPP
#define TREE_HPP

#include <string>

#include "constants.hpp"


/* class TypeError: public std::exception {
    std::string msg;
public:
    TypeError(const std::string& msg=""): msg{msg} {};
    const char* what() const noexcept override {return msg.c_str();};
}; */

class Node {
    constants::Types type;
    std::string value;
    Node *next;
    Node *child;
    Node *parent;
    Node *copyNodeAndChilds();
    void displayTree(std::ostream &flow = std::cout, int level=0) const;
public :
    Node(constants::Types type=constants::Types::NUL, std::string value="", Node *parent=nullptr): type{type}, value{value}, next{}, child{}, parent{parent} {};
    virtual ~Node();
    void setValue(const std::string& s) {value = s;};
    virtual const std::string& getValue() const {return value;};
    void setType(const constants::Types& t) {type = t;};
    const constants::Types& getType() const {return type;};
    Node *getParent() const {return parent;};
    void setParent(Node *parent);
    Node *getNext() const {return next;};
    void setNext(Node *next);
    void removeNext() {setValue(nullptr);};
    Node *getChild() const {return child;};
    /*Set the child pointer*/
    void setChild(Node *child);
    void removeChild() {setChild(nullptr);};
    void appendNext(Node *next);
    /*add a child at the end of them*/
    Node *appendChild(Node *child);
    Node *appendChild(Node &child);
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
    void display(std::ostream &flow = std::cout) const;
    std::string str() const;

    friend bool operator==(const Node &, const Node &);
    friend std::ostream &operator<<(std::ostream &o, const Node &n);
};

/**
    *Return the parent who don't have a parent
*/
Node *root(Node *node);

Node *getLastChild(Node *n);

int getPriority(const std::string &ope);

bool areSameNodes(const Node *node1, const Node *node2);

#endif // TREE_HPP