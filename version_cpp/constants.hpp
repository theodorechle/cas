#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <iostream>
#include <string>

using namespace std;

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

ostream& operator<<(ostream& o, const Types type);

const string ADDITION_SIGN{"+"};
const string SUBSTRACTION_SIGN = {"-"};
const string MULTIPLICATION_SIGN = {"*"};
const string DIVISION_SIGN = {"/"};
const string POWER_SIGN = {"**"};
const string OPENING_PARENTHESIS_SIGN = {"("};
const string CLOSING_PARENTHESIS_SIGN = {")"};
const string IMPLICIT_MULTIPLICATION_SIGN = {""};

const int ADDITION_PRIORITY = 1;
const int SUBSTRACTION_PRIORITY = 1;
const int MULTIPLICATION_PRIORITY = 2;
const int DIVISION_PRIORITY = 2;
const int POWER_PRIORITY = 3;
const int IMPLICIT_MULTIPLICATION_PRIORITY = 4;

constexpr int NB_OPE = 8;

const string MULTIPLE_CHARS_OPERATORS[] = {POWER_SIGN};
const string OPERATORS[NB_OPE] = {ADDITION_SIGN, SUBSTRACTION_SIGN, MULTIPLICATION_SIGN, DIVISION_SIGN, POWER_SIGN};

}

#endif // CONSTANTS_HPP