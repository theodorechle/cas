#ifndef ADDITION_HPP
#define ADDITION_HPP

#include <string>

#include "operator.hpp"

class Addition: public Operator {

public:
    Addition(Node *parent=nullptr)
    : Operator{Token::Plus, parent} {};

    bool solve(bool *delete_self);
    Node *createNewNode() const override {return new Addition();}
};

#endif // ADDITION_HPP