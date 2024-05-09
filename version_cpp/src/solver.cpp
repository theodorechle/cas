#include "solver.hpp"
#include "parser.hpp"
#include "addition.hpp"

void sortTree(Node* node) {
    
}





Node *solve(Node *expr, bool debug) {
    bool updated = true;
    while (updated || !Parser::isNodeNull(expr->getParent())) {
        updated = false;
    }

    return expr;
}