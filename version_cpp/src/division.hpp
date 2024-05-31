#ifndef DIVISION_HPP
#define DIVISION_HPP

#include <string>

#include "operator.hpp"

class Division: public Operator {

public:
    Division(Node *parent=nullptr)
    : Operator{Token::Slash, parent} {};

    bool solve(bool *delete_self);
};

#endif // DIVISION_HPP