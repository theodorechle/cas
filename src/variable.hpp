#ifndef VARIABLE_HPP
#define VARIABLE_HPP

#include <string>

#include "node.hpp"

class Variable: public Node {
public:
    Variable(const std::string &value, Node *parent = nullptr): Node{Token::Variable, value, parent} {}
};

#endif // VARIABLE_HPP