#include <stdbool.h>
#include <ctype.h>

#include "constants.h"
#ifndef importedTree
#include "tree.h"
#define importedTree
#endif

bool isTypeOrEmpty(int type, int checkType);

bool isFunction(char* litteral);

bool isOperator(char* value);

TreeList* addTreeByValues(TreeList* exprList, char* value, int type);

TreeList* parser(char* expr, bool debug, bool implicitPriority);

Tree findRootOrParenthesis(Tree tree);

/**
 * Transform a list (like the ones the parser function returns) into a tree containing the entire expression in the list
*/
Tree parsedToTree(TreeList* exprList, bool debug, bool implicitPriority);