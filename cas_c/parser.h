#include <stdbool.h>
#include <ctype.h>

#include "constants.h"
#include "tree.h"

bool isTypeOrEmpty(int type, int checkType);

bool isFunction(char* litteral);

bool isOperator(String* value);

TreeList* addTreeByValues(TreeList* exprList, String* value, int type);

TreeList* parser(char* expr, bool debug, bool implicitPriority);

Tree* findRootOrParenthesis(Tree* tree);

/**
 * Transform a list (like the ones the parser function returns) into a tree containing the entire expression in the list
*/
Tree* parsedToTree(TreeList* exprList, bool debug, bool implicitPriority);