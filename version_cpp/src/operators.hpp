#ifndef OPERATORS_HPP
#define OPERATORS_HPP

#include "tokens.hpp"
#include <unordered_map>

// operators and their priorities
std::unordered_map<Token, int> OPERATORS {
    {Token::Plus, 1},
    {Token::Minus, 1},
    {Token::Times, 2},
    {Token::Slash, 2},
    {Token::Caret, 3},
    {Token::ImplicitTimes, 4}
};

const int DEFAULT_PRIORITY = 50;


#endif // OPERATORS_HPP