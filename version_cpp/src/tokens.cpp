#include <string>

#include "tokens.hpp"

std::string TokensStr(const Token type) {
    std::string value;
    switch (type) {
        case Token::NullRoot: value = "NullRoot";break;
        case Token::Empty: value = "Empty";break;
        case Token::Name: value = "Variable";break;
        case Token::Number: value = "Number";break;
        case Token::Operator: value = "Operator";break;
        case Token::OpeningParenthesis: value = "OpeningParenthesis";break;
        case Token::ClosingParenthesis: value = "ClosingParenthesis";break;
        case Token::Function: value = "Function";break;
    }
    return value;
}

std::ostream& operator<<(std::ostream& o, const Token type) {
    o << TokensStr(type);
    return o;
}

