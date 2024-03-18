#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <iostream>
#include <string>
#include <map>

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
const std::string POWER_SIGN{"^"};
const std::string POWER_SIGN_2{"**"};
const std::string OPENING_PARENTHESIS_SIGN{"("};
const std::string CLOSING_PARENTHESIS_SIGN{")"};
const std::string IMPLICIT_MULTIPLICATION_SIGN{""};

const std::map<std::string, std::string> REPLACE_OPERATORS {
    {POWER_SIGN_2, POWER_SIGN}
};

}

#endif // CONSTANTS_HPP