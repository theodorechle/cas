#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <iostream>
#include <string>

namespace constants {

enum class Types {
    NUL,
    VAR,
    NBR,
    OPA,
    CPA,
    FUC
};

std::ostream& operator<<(std::ostream& o, const Types type);

const std::string OPENING_PARENTHESIS_SIGN{"("};
const std::string CLOSING_PARENTHESIS_SIGN{")"};
const std::string IMPLICIT_MULTIPLICATION_SIGN{""};

}

#endif // CONSTANTS_HPP