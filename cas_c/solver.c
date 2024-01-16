#include "solver.h"

bool addition(Tree node) {
    Tree node1 = getChild(node, 0);
    Tree node2 = getChild(node, 1);
    int typeN1 = getType(node1), typeN2 = getType(node2);
    Tree newTree = createTree();
    char* s = (char*)malloc(sizeof(double));
    if (typeN1 == typeN2 && typeN1 == TYPE_NUMBER) {
        sprintf(s, "%f", atof(getValue(node1)) + atof(getValue(node2)));
        setValue(newTree, s);
        setType(newTree, TYPE_NUMBER);
    }
    else return false;
    replaceTree(node, newTree);
    return true;
}

bool substraction(Tree node) {

}

bool multiplication(Tree node) {

}

bool division(Tree node) {

}

bool power(Tree node) {

}

Tree goToLeaf(Tree node) {
    if (isEmptyTree(node) || getNbChilds(node) == 0) return node;
    Tree child = getChild(node, 0);
    while (getNbChilds(child)) {
        node = child;
        child = getChild(child, 0);
    }
    return child;
}

Tree goToNextChild(Tree node) {
    if (isEmptyTree(node)) return node;
    while (!isEmptyTree(getParent(node))) {
        if (getNbChilds(getParent(node)) - 1 > node->childIndex)
            return getChild(getParent(node), node->childIndex + 1);
        else node = getParent(node);
    }
    return node;
}

Tree solve(Tree expr, bool debug) {
    /**
     * Go to the parent of the first leaf, solve it, if not modified, solve it's parent, else solve his first leaf's parent
    */
    bool change = true;
    char* value;
    if (treeLength(expr) == 1) return expr;
    expr = getParent(goToLeaf(expr)); // go to first expression to evaluate : the father of the first leaf, because the leaf itself can't be evaluated
    while (!isEmptyTree(getParent(expr)) || change == true) {
        if (getType(expr) != TYPE_OPERATOR) {
            expr = getParent(expr);
            continue;
        }
        value = getValue(expr);
        if (strcmp(value, ADDITION_SIGN) == 0) change = addition(expr);
        if (strcmp(value, SUBSTRACTION_SIGN) == 0) change = substraction(expr);
        if (strcmp(value, MULTIPLICATION_SIGN) == 0) change = multiplication(expr);
        if (strcmp(value, DIVISION_SIGN) == 0) change = division(expr);
        if (strcmp(value, POWER_SIGN) == 0) change = power(expr);
        if (debug) printTree(findRoot(expr));
        if (change) {
            if (isEmptyTree(getParent(expr))) return expr;
            expr = getParent(goToLeaf(expr));
        }
        else expr = goToNextChild(expr);
    }
    return expr;
}