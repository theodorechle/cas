#ifndef TOKENS_HPP
#define TOKENS_HPP

enum class Token {
    Name,
    Number,
    Operator,
    Function,
    OpeningParenthesis,
    ClosingParenthesis,
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
     * Nodes with this type will be ignored by the root and getRootOrParenthesis functions
     * (they return the NullRoot node's child)
    */
   NullRoot
};

std::string TokensStr(const Token type);

std::ostream& operator<<(std::ostream& o, const Token type);

#endif // TOKENS_HPP