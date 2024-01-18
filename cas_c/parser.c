#include "parser.h"

bool isTypeOrEmpty(int type, int checkType) {
    return (!type || type == checkType);
}

bool isFunction(char* litteral) {
    return false; // to change
}

bool isOperator(String value) {
    for (int i=0; i < NB_OPE; i++)
        if (strcmp(value->str, OPERATORS[i]) == 0)
            return true;
    return false;
}


TreeList* addTreeByValues(TreeList* exprList, String value, int type) {
    if (type == TYPE_NULL) {
        fprintf(stderr, "Error in addTreeByValues : Unknown value '%s'\n", value->str);
        exit(5);
    }
    Tree tree = createTree();
    setValue(tree, value->str);
    setType(tree, type);
    return addTree(exprList, tree);
}

TreeList* parser(char* expr, bool debug, bool implicitPriority) {
    TreeList* exprList = createTreeList();
    String value = createString(), testString = createString();
    bool createNewTree;
    char character;
    int type = 0, length = strlen(expr), index = 0;
    while (index < length) {
        createNewTree = true;
        character = expr[index];
        if (debug) printf("character : %c\n", character);
        if (isspace(character)) {
            if (debug) printf("Found type of character '%c' is space\n", character);
            if (isEmptyValue(value))
                createNewTree = false;
        }
        else if (isalpha(character)) {
            if (debug) printf("Found type of character '%c' is alpha\n", character);
            if (isTypeOrEmpty(type, TYPE_VARIABLE)) {
                appendCharToString(value, character);
                type = TYPE_VARIABLE;
                createNewTree = false;
            }
            else {
                if (type == TYPE_NUMBER || type == TYPE_CLOSING_PARENTHESIS) {
                    exprList = addTreeByValues(exprList, value, type);
                    clearString(value);
                    if (implicitPriority) appendToString(value, IMPLICIT_MULTIPLICATION_SIGN);
                    else appendToString(value, MULTIPLICATION_SIGN);
                    type = TYPE_OPERATOR;
                }
                index--;
            }
        }
        else if (character == OPENING_PARENTHESIS_SIGN[0]) {
            if (debug) printf("Found type of character '%c' is opening parenthesis\n", character);
            if (!type) type = TYPE_OPENING_PARENTHESIS;
            else {
                if (type == TYPE_NUMBER || type == TYPE_VARIABLE) {
                    exprList = addTreeByValues(exprList, value, type);
                    clearString(value);
                    if (implicitPriority) appendToString(value, IMPLICIT_MULTIPLICATION_SIGN);
                    else appendToString(value, MULTIPLICATION_SIGN);
                    type = TYPE_OPERATOR;
                }
                index--;
            }
        }
        else if (character == CLOSING_PARENTHESIS_SIGN[0]) {
            if (debug) printf("Found type of character '%c' is closing parenthesis\n", character);
            if (!type) type = TYPE_CLOSING_PARENTHESIS;
            else index --;
        }
        else if (isdigit(character) || character == '.') {
            if (debug) printf("Found type of character '%c' is number\n", character);
            if (isTypeOrEmpty(type, TYPE_NUMBER)) {
                appendCharToString(value, character);
                type = TYPE_NUMBER;
                createNewTree = false;
            }
            else if (type == TYPE_VARIABLE) {
                appendCharToString(value, character);
                createNewTree = false;
            }
            else index --;
        }
        else
            if (type) index--;
            else {
                appendToString(testString, value->str);
                appendCharToString(testString, character);
                if (isOperator(testString)) {
                    type = TYPE_OPERATOR;
                    clearString(value);
                    appendToString(value, testString->str);
                    if (debug) printf("Found type of value '%s' is operator\n", value->str);
                }
                else
                    if (debug) printf("Type of character '%c' not found\n", character);
                createNewTree = false;
                clearString(testString);
            }
        if (debug) printf("value : %s type : %d\n", value->str, type);
        if (createNewTree) {
            exprList = addTreeByValues(exprList, value, type);
            clearString(value);
            type = TYPE_NULL;
        }
        index++;
    }
    if (!isEmptyValue(value))
        exprList = addTreeByValues(exprList, value, type);
    return exprList;
}

Tree findRootOrParenthesis(Tree tree) {
    if (getParent(tree) == NULL || getType(getParent(tree)) == TYPE_OPENING_PARENTHESIS) return tree;
    return findRootOrParenthesis(getParent(tree));
}

Tree parsedToTree(TreeList* exprList, bool debug, bool implicitPriority) {
    Tree tree=createTree(), t, child;
    int ttype;
    while (exprList != NULL) {
        t = exprList->tree;
        ttype = getType(t);
        if (ttype == TYPE_NUMBER || ttype == TYPE_VARIABLE) {
            replaceTree(tree, t);
            tree = findRootOrParenthesis(tree);
        }
        else if (ttype == TYPE_OPERATOR) {
            if (getType(tree) == TYPE_NULL && getValue(t) == SUBSTRACTION_SIGN)
                replaceTree(tree, setType(setValue(createTree(), "0"), TYPE_NUMBER));
            if (getType(tree) != TYPE_OPERATOR || getPriority(getValue(t)) <= getPriority(getValue(tree))) {
                replaceTree(addEmptyChild(t), tree);
                replaceTree(tree, t);
                tree = addEmptyChild(tree);
            }
            else {
                child = getChild(tree, -1);
                while (getType(child) == TYPE_OPERATOR && getPriority(getValue(t)) > getPriority(getValue(child)))
                    child = getChild(child, -1);
                if (!strcmp(getValue(t), IMPLICIT_MULTIPLICATION_SIGN)) setValue(t, MULTIPLICATION_SIGN);
                replaceTree(addEmptyChild(t), child);
                tree = addEmptyChild(replaceTree(getChild(getParent(child), -1), t));
            }
        }
        else if (ttype == TYPE_OPENING_PARENTHESIS) {
            if (getType(tree) != TYPE_NULL) tree = addEmptyChild(tree);
            replaceTree(tree, t);
            tree = addEmptyChild(tree);
        }
        else if (ttype == TYPE_CLOSING_PARENTHESIS) {
            replaceTree(getParent(tree), tree);
            tree = findRootOrParenthesis(tree);
        }
        if (debug) {
            printf("\nRoot : \n");
            printTree(findRoot(tree));
            printf("\n");
        }
        exprList = exprList->next;
    }
    return findRoot(tree);
}
