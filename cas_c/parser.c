#include "parser.h"

bool isTypeOrEmpty(int type, int checkType) {
    return (!type || type == checkType);
}

bool isFunction(char* litteral) {
    return false; // to change
}

bool isOperator(String* value) {
    for (int i=0; i < NB_OPE; i++) {
        printf("%s == %s : %d\n", value->str, OPERATORS[i], strcmp(value->str, OPERATORS[i]) == 0);
        if (strcmp(value->str, OPERATORS[i]) == 0)
            return true;
    }
    return false;
}

TreeList* addTreeByValues(TreeList* exprList, String* value, int type) {
    if (type == TYPE_NULL) {
        fprintf(stderr, "Error in addTreeByValues : Unknown value '%s'\n", value->str);
        exit(5);
    }
    Tree* tree = createTree();
    setValue(tree, value->str);
    setType(tree, type);
    return addTree(exprList, tree);
}

TreeList* parser(char* expr, bool debug) {
    TreeList* exprList = createTreeList();
    String* value = createString(), *testString = createString();
    bool createNewTree;
    char character;
    int type = 0, length = strlen(expr), index = 0;
    while (index < length) {
        createNewTree = true;
        character = expr[index];
        if (isspace(character)) {
            if (debug) printf("Found type of character '%c' is space\n", character);
            if (!value->size)
                createNewTree = false;
        }
        else if (isalpha(character)) {
            if (debug) printf("Found type of character '%c' is alpha\n", character);
            if (isTypeOrEmpty(type, TYPE_VARIABLE)) {
                appendCharToString(value, character);
                type = TYPE_VARIABLE;
                createNewTree = false;
            }
            else if (type == TYPE_NUMBER || type == TYPE_CLOSING_PARENTHESIS) {
                exprList = addTreeByValues(exprList, value, type);
                clearString(value);
                appendToString(value, MULTIPLICATION);
            }
            else index--;
        }
        else if (character == OPENING_PARENTHESIS[0]) {
            if (debug) printf("Found type of character '%c' is opening parenthesis\n", character);
            if (!type) type = TYPE_OPENING_PARENTHESIS;
            else if (type == TYPE_NUMBER || type == TYPE_VARIABLE)
            {
                exprList = addTreeByValues(exprList, value, type);
                clearString(value);
                appendToString(value, MULTIPLICATION);
            }
            else index--;
        }
        else if (character == CLOSING_PARENTHESIS[0]) {
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
        }
        else {
            if (type) index--;
            else {
                appendToString(testString, value->str);
                appendCharToString(testString, character);
                if (isOperator(value)) {
                    type = TYPE_OPERATOR;
                    if (debug) printf("Found type of value '%s' is operator\n", value->str);
                }
                else
                    if (debug) printf("Type of character '%c' not found\n", character);
                createNewTree = false;
                clearString(testString);
            }
        }
        if (debug)
            printf("value : %s type : %d\n", value->str, type);
        if (createNewTree) {
            exprList = addTreeByValues(exprList, value, type);
            clearString(value);
            type = TYPE_NULL;
        }
        index++;
    }
    return exprList;
}