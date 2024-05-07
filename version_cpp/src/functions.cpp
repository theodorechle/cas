#include "functions.hpp"

const std::unordered_map<std::string, bool (*)(Node *)> FUNCTIONS {
    {"abs", absolute}
};