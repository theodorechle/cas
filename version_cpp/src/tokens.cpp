#include <string>

#include "tokens.hpp"

std::string tokenToStr(const Token &type) {
    std::string value;
    switch (type) {
        case Token::Name: value = "Name";break;
        case Token::Space: value = "Space";break;
        case Token::Variable: value = "Variable";break;
        case Token::Number: value = "Number";break;
        case Token::Function: value = "Function";break;
        case Token::OpeningParenthesis: value = "OpeningParenthesis";break;
        case Token::ClosingParenthesis: value = "ClosingParenthesis";break;
        case Token::Plus: value = "Plus";break;
        case Token::Minus: value = "Minus";break;
        case Token::Times: value = "Times";break;
        case Token::ImplicitTimes: value = "ImplicitTimes";break;
        case Token::Slash: value = "Slash";break;
        case Token::DoubleTimes: value = "DoubleTimes";break;
        case Token::Caret: value = "Caret";break;
        case Token::Comma: value = "Comma";break;
        case Token::Bang: value = "Bang";break;
        case Token::Empty: value = "Empty";break;
        case Token::NullRoot: value = "NullRoot";break;
        default: value = "Unknown";break;
    }
    return value;
}

const std::unordered_map<Token, int> Operators::OperatorsPriorities {
    {Token::Plus, 1},
    {Token::Minus, 1},
    {Token::Times, 2},
    {Token::Slash, 2},
    {Token::ImplicitTimes, 3},
    {Token::Caret, 4}
};

const std::unordered_map<Token, std::string> Operators::OperatorsStrings {
    {Token::Plus, "+"},
    {Token::Minus, "-"},
    {Token::Times, "*"},
    {Token::Slash, "/"},
    {Token::Caret, "^"},
    {Token::DoubleTimes, "**"}
};

int getOperatorPriority(const Token &token) {
    const std::unordered_map<Token, int>::const_iterator iter = Operators::OperatorsPriorities.find(token);
    if (iter != Operators::OperatorsPriorities.cend()) return iter->second;
    return Operators::DEFAULT_PRIORITY;
}

std::string OperatorsString(const Token &token) {
    const std::unordered_map<Token, std::string>::const_iterator iter = Operators::OperatorsStrings.find(token);
    if (iter != Operators::OperatorsStrings.cend()) return iter->second;
    return "";
}

std::ostream& operator<<(std::ostream& o, const Token type) {
    o << tokenToStr(type);
    return o;
}

