#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include <unordered_map>
#include <string>

#include "node.hpp"
#include "absolute.hpp"
#include "sinus.hpp"

typedef bool (*function)(Node *);

const std::unordered_map<std::string, function> FUNCTIONS{
    {"abs", absolute},
    {"sin", sinus}
};

bool (*getFunction(const std::string &value))(Node *);
bool isFunction(const std::string &value);

#endif // FUNCTIONS_HPP