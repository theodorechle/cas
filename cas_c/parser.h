#include <stdbool.h>
#include <ctype.h>

#include "constants.h"
#include "tree.h"

bool isTypeOrEmpty(int type, int checkType);

bool isFunction(char* litteral);

TreeList* addTreeByValues(TreeList* exprList, String* value, int type);

TreeList* parser(char* expr, bool debug);

bool isOperator(String* value);
