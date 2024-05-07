#include "functions.hpp"

bool (*getFunction(const std::string &name))(Node *) {
    std::unordered_map<std::string, bool (*)(Node *)>::const_iterator iter = FUNCTIONS.find(name);
    if (iter != FUNCTIONS.cend()) return iter->second;
    return nullptr;
}


bool isFunction(const std::string &name) {
    std::unordered_map<std::string, bool (*)(Node *)>::const_iterator iter = FUNCTIONS.find(name);
    return (iter != FUNCTIONS.cend());
}