#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <iostream>
#include <string>
#include <map>
#include <unordered_map>

namespace constants {

enum class Types {
    /**NullRoot is used only when an algorithm must start with an existing node
     * Nodes with this type will be ignored by the root and getRootOrParenthesis functions
     * (they return the NullRoot node's child)
    */
    NullRoot,
    Empty,
    Variable,
    Number,
    Operator,
    OpeningParenthesis,
    ClosingParenthesis,
    Function
};

std::string TypesStr(const Types type);

std::ostream& operator<<(std::ostream& o, const Types type);

const std::string ADDITION_SIGN{"+"};
const std::string SUBSTRACTION_SIGN{"-"};
const std::string MULTIPLICATION_SIGN{"*"};
const std::string DIVISION_SIGN{"/"};
const std::string POWER_SIGN{"^"};
const std::string POWER_SIGN_2{"**"};
const std::string OPENING_PARENTHESIS_SIGN{"("};
const std::string CLOSING_PARENTHESIS_SIGN{")"};
const std::string IMPLICIT_MULTIPLICATION_SIGN{""};

const std::map<std::string, std::string> REPLACE_OPERATORS {
    {POWER_SIGN_2, POWER_SIGN}
};

// operators and their priorities
const std::unordered_map<std::string, int> OPERATORS {
    {constants::ADDITION_SIGN, 1},
    {constants::SUBSTRACTION_SIGN, 1},
    {constants::MULTIPLICATION_SIGN, 2},
    {constants::DIVISION_SIGN, 2},
    {constants::POWER_SIGN, 3},
    {constants::IMPLICIT_MULTIPLICATION_SIGN, 4}
};

const int DEFAULT_PRIORITY = 50;

int getPriority(const std::string &ope);

}

#endif // CONSTANTS_HPP