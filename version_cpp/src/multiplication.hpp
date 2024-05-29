#ifndef MULTIPLICATION_HPP
#define MULTIPLICATION_HPP

#include <string>

#include "operator.hpp"

class Multiplication: public Operator {

public:
    Multiplication(Node *parent=nullptr)
    : Operator{Token::Plus, parent} {};

    bool solve(bool *delete_self);
};

#endif // MULTIPLICATION_HPP