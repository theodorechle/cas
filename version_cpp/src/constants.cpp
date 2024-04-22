#include "constants.hpp"

namespace constants {

std::string TypesStr(const Types type) {
    std::string value;
    switch (type) {
        case Types::NullRoot: value = "NullRoot";break;
        case Types::Empty: value = "Empty";break;
        case Types::Variable: value = "Variable";break;
        case Types::Number: value = "Number";break;
        case Types::Operator: value = "Operator";break;
        case Types::OpeningParenthesis: value = "OpeningParenthesis";break;
        case Types::ClosingParenthesis: value = "ClosingParenthesis";break;
        case Types::Function: value = "Function";break;
    }
    return value;
}

std::ostream& operator<<(std::ostream& o, const Types type) {
    o << TypesStr(type);
    return o;
}

int getPriority(const std::string &ope) {
    std::unordered_map<std::string, int>::const_iterator iter = OPERATORS.find(ope);
    if (iter != OPERATORS.cend()) return iter->second;
    return DEFAULT_PRIORITY;
}
}
