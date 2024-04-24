#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include <unordered_map>
#include <string>

#include "node.hpp"

const std::unordered_map<std::string, bool (*)(Node *)> FUNCTIONS {

};

bool (*getFunction(const std::string &value))(Node *) {
    std::unordered_map<std::string, bool (*)(Node *)>::const_iterator iter = FUNCTIONS.find(value);
    if (iter != FUNCTIONS.cend()) return iter->second;
    return nullptr;
}


#endif // FUNCTIONS_HPP