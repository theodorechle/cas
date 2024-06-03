#include "operator.hpp"

bool areSameTokens(Token token1, Token token2, Token expected1, Token expected2) {
    return (token1 == expected1 && token2 == expected2) ||
            (token1 == expected2 && token2 == expected1);
}

bool isOneTokenTheSame(Token token1, Token token2, Token expected) {
    return token1 == expected || token2 == expected;
}
