#include "operator.hpp"

bool areSameTokens(Token token1, Token token2, Token expected1, Token expected2) {
    return (token1 == expected1 && token2 == expected2) ||
            (token1 == expected2 && token2 == expected1);
}

bool isOneTokenTheSame(Token token1, Token token2, Token expected) {
    return token1 == expected || token2 == expected;
}


const std::unordered_map<Token, int> Operator::OperatorsPriorities {
    {Token::Plus, 1},
    {Token::Minus, 1},
    {Token::Times, 2},
    {Token::Slash, 2},
    {Token::ImplicitTimes, 3},
    {Token::Caret, 4}
};

const std::unordered_map<Token, std::string> Operator::OperatorsStrings {
    {Token::Plus, "+"},
    {Token::Minus, "-"},
    {Token::Times, "*"},
    {Token::Slash, "/"},
    {Token::Caret, "^"},
    {Token::DoubleTimes, "**"}
};

int getOperatorPriority(const Token &token) {
    const std::unordered_map<Token, int>::const_iterator iter = Operator::OperatorsPriorities.find(token);
    if (iter != Operator::OperatorsPriorities.cend()) return iter->second;
    return Operator::DEFAULT_PRIORITY;
}

std::string OperatorsString(const Token &token) {
    const std::unordered_map<Token, std::string>::const_iterator iter = Operator::OperatorsStrings.find(token);
    if (iter != Operator::OperatorsStrings.cend()) return iter->second;
    return "";
}

std::ostream& operator<<(std::ostream& o, const Token type) {
    o << tokenToString(type);
    return o;
}
