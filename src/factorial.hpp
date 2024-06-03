#ifndef FACTORIAL_HPP
#define FACTORIAL_HPP

#include <string>

#include "operator.hpp"

class Factorial: public Operator {

public:
    Factorial(Node *parent=nullptr)
    : Operator{Token::Bang, parent} {};

    bool solve(bool *delete_self);
};

#endif // FACTORIAL_HPP