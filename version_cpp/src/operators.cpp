#include "operators.hpp"

int getPriority(const Token ope) {
    std::unordered_map<Token, int>::const_iterator iter = OPERATORS.find(ope);
    if (iter != OPERATORS.cend()) return iter->second;
    return DEFAULT_PRIORITY;
}