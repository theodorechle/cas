#ifndef TOKENS_HPP
#define TOKENS_HPP

#include <unordered_map>

enum class Token {
    Name,
    Number,
    Function,
    Variable,
    OpeningParenthesis,
    ClosingParenthesis,
    Space,
    Plus,
    Minus,
    Times,
    ImplicitTimes,
    Slash,
    DoubleTimes,
    Caret,
    Comma,
    Bang,
    Empty,
    /**NullRoot is used only when an algorithm must start with an existing node
     * Nodes with this type will be ignored by the root and getRootOrStopBeforeParenthesis functions
     * (they return the NullRoot node's child)
    */
   NullRoot
};

class Operators {
public:
    static const int DEFAULT_PRIORITY = INT32_MAX;
    // operators and their priorities
    static const std::unordered_map<Token, int> OperatorsPriorities;
    // operators and the strings associated to
    static const std::unordered_map<Token, std::string> OperatorsStrings;
};


std::string tokenToStr(const Token &type);

int getOperatorPriority(const Token &token);

/**
 * Take a token and if it is an operator, return a char * associated to.
 * Ex :
 * OperatorString(Token::Plus) // "+"
*/
std::string OperatorsString(const Token &token);

std::ostream& operator<<(std::ostream& o, const Token type);

#endif // TOKENS_HPP