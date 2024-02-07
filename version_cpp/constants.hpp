#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

using namespace std;

namespace constants {

enum class Types {
    NUL,
    VAR,
    NBR,
    OPT,
    OPA,
    CPA,
    FUC,
};

const string ADDITION_SIGN = "+";
const string SUBSTRACTION_SIGN = "-";
const string MULTIPLICATION_SIGN = "*";
const string DIVISION_SIGN = "/";
const string POWER_SIGN = "**";
const string OPENING_PARENTHESIS_SIGN = "(";
const string CLOSING_PARENTHESIS_SIGN = ")";
const string IMPLICIT_MULTIPLICATION_SIGN = "";

constexpr int NB_OPE = 8;

const string MULTIPLE_CHARS_OPERATORS[] = {POWER_SIGN};
const string OPERATORS[NB_OPE] = {ADDITION_SIGN, SUBSTRACTION_SIGN, MULTIPLICATION_SIGN, DIVISION_SIGN, POWER_SIGN};

}

#endif // CONSTANTS_HPP