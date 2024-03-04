#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include <unordered_map>
#include <string>
#include <bits/stdc++.h>

#include "node.hpp"
#include "constants.hpp"

bool add(Node *node);
bool substract(Node *node);
bool multiply(Node *node);
bool divide(Node *node);
bool power(Node *node);

std::unordered_map<std::string, std::function<bool(Node *)>> FUNCTIONS {
    {"+", add},
    {"-", substract},
    {"*", multiply},
    {"/", divide},
    {"**", power},
};


std::unordered_map<std::string, int> PRIORITIES {
    {"+", 1},
    {"-", 1},
    {"*", 2},
    {"/", 2},
    {"**", 3},
    {IMPLICIT_MULTIPLICATION_SIGN, 4}
};

constexpr int DEFAULT_PRIORITY = 25; // must be greater than all the other priorities

#endif // FUNCTIONS_HPP