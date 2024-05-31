#ifndef POWER_HPP
#define POWER_HPP

#include <string>

#include "operator.hpp"

class Power: public Operator {

public:
    Power(Node *parent=nullptr)
    : Operator{Token::Caret, parent} {};

    bool solve(bool *delete_self);
};

#endif // POWER_HPP