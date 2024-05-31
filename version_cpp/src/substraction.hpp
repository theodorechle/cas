#ifndef SUBSTRACTION_HPP
#define SUBSTRACTION_HPP

#include <string>

#include "operator.hpp"

class Substraction: public Operator {

public:
    Substraction(Node *parent=nullptr)
    : Operator{Token::Minus, parent} {};

    bool solve(bool *delete_self);
};

#endif // SUBSTRACTION_HPP