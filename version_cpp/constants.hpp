#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <iostream>
#include <string>

namespace constants {

enum class Types {
    NUL,
    VAR,
    NBR,
    OPT,
    OPA,
    CPA,
    FUC
};

std::ostream& operator<<(std::ostream& o, const Types type);

const std::string ADDITION_SIGN{"+"};
const std::string SUBSTRACTION_SIGN{"-"};
const std::string MULTIPLICATION_SIGN{"*"};
const std::string DIVISION_SIGN{"/"};
const std::string POWER_SIGN{"**"};
const std::string OPENING_PARENTHESIS_SIGN{"("};
const std::string CLOSING_PARENTHESIS_SIGN{")"};
const std::string IMPLICIT_MULTIPLICATION_SIGN{""};

const int ADDITION_PRIORITY = 1;
const int SUBSTRACTION_PRIORITY = 1;
const int MULTIPLICATION_PRIORITY = 2;
const int DIVISION_PRIORITY = 2;
const int POWER_PRIORITY = 3;
const int IMPLICIT_MULTIPLICATION_PRIORITY = 4;

constexpr int NB_OPE = 8;

const std::string MULTIPLE_CHARS_OPERATORS[] = {POWER_SIGN};
const std::string OPERATORS[NB_OPE] = {ADDITION_SIGN, SUBSTRACTION_SIGN, MULTIPLICATION_SIGN, DIVISION_SIGN, POWER_SIGN};

}

#endif // CONSTANTS_HPP