#ifndef importedTree
#include "tree.h"
#define importedTree
#endif

void findSuperiorGroup(Tree node, int priority, TreeList* nodes);

bool addition(Tree node);

bool substraction(Tree node);

bool multiplication(Tree node);

bool division(Tree node);

bool power(Tree node);

Tree goToLeaf(Tree node);

/**
 * Go up in the tree and returns the next child to solve (use the childIndex)
*/
Tree goToNextExpr(Tree node, bool* change);

Tree solve(Tree expr, bool debug);