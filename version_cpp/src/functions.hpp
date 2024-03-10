#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include <unordered_map>
#include <map>
#include <set>
#include <string>
#include <bits/stdc++.h>

#include "node.hpp"
#include "constants.hpp"

bool add(Node *node);
bool substract(Node *node);
bool multiply(Node *node);
bool divide(Node *node);
bool power(Node *node);

const std::unordered_map<std::string, std::function<bool(Node *)>> FUNCTIONS {
    {constants::ADDITION_SIGN, add},
    {constants::SUBSTRACTION_SIGN, substract},
    {constants::MULTIPLICATION_SIGN, multiply},
    {constants::DIVISION_SIGN, divide},
    {constants::POWER_SIGN, power},
    {constants::POWER_SIGN_2, power},
};

// OPERATORS is only for the display to remove parenthesis
const std::set<std::string> OPERATORS {
    constants::ADDITION_SIGN,
    constants::SUBSTRACTION_SIGN,
    constants::MULTIPLICATION_SIGN,
    constants::DIVISION_SIGN,
    constants::POWER_SIGN,
    constants::POWER_SIGN_2
};

const std::map<std::string, int> PRIORITIES {
    {constants::ADDITION_SIGN, 1},
    {constants::SUBSTRACTION_SIGN, 1},
    {constants::MULTIPLICATION_SIGN, 2},
    {constants::DIVISION_SIGN, 2},
    {constants::POWER_SIGN, 3},
    {constants::POWER_SIGN_2, 3},
    {constants::IMPLICIT_MULTIPLICATION_SIGN, 4}
};

constexpr int DEFAULT_PRIORITY = 25; // must be greater than all the other priorities

#endif // FUNCTIONS_HPP