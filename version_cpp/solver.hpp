#ifndef SOLVER_HPP
#define SOLVER_HPP

#include "node.hpp"

void findSuperiorGroup(Node *node, int priority, Node *nodes);

bool addition(Node *node);

bool substraction(Node *node);

bool multiplication(Node *node);

bool division(Node *node);

bool power(Node *node);

Node *goToLeaf(Node *node);

/**
  *Go up in the tree and returns the next child to solve (use the childIndex)
*/
Node *goToNextExpr(Node *node, bool *change);

Node *solve(Node *expr, bool debug);

#endif // SOLVER_HPP