#include "solver.h"

TreeList* getVars(Tree t, TreeList* vars) {
    if (getType(t) == TYPE_VARIABLE) vars = addTree(vars, t);
    TreeList* childs = t->childs;
    while (childs != NULL) {
        vars = getVars(childs->tree, vars);
        childs = childs->next;
    }
    return vars;
}

bool addition(Tree node) {
    printf("node\n");
    printTree(node);
    Tree node1 = getChild(node, 0);
    Tree node2 = getChild(node, 1);
    int typeN1 = getType(node1), typeN2 = getType(node2);
    Tree newTree = createTree();
    TreeList* vars1 = createTreeList(), *vars2 = createTreeList();
    Tree mul1, mul2, child;
    char* s = (char*)malloc(sizeof(double));
    if (typeN1 == typeN2)
        if (typeN1 == TYPE_NUMBER) {
            sprintf(s, "%f", atof(getValue(node1)) + atof(getValue(node2)));
            setValue(newTree, s);
            setType(newTree, TYPE_NUMBER);
            replaceTree(node, newTree);
            return true;
        }
        else {
            vars1 = getVars(node1, vars1);
            vars2 = getVars(node2, vars2);
            if (checkSameVars(vars1, vars2)) {
                mul1 = getMultiplicator(node1);
                mul2 = getMultiplicator(node2);
                // if no multiplicator, set it to 1
                // example : a*b => 1*a*b
                if (isEmptyTree(mul1)) {
                    mul1 = setType(setValue(createTree(), "1"), TYPE_NUMBER);
                    replaceTree(addEmptyChild(newTree), node1);
                    addChild(newTree, mul1);
                    setType(newTree, TYPE_OPERATOR);
                    setValue(newTree, MULTIPLICATION_SIGN);
                    replaceTree(node1, newTree);
                }
                if (isEmptyTree(mul2)) mul2 = setType(setValue(createTree(), "1"), TYPE_NUMBER);
                newTree = createTree();
                replaceTree(addEmptyChild(newTree), mul1);
                setValue(newTree, ADDITION_SIGN);
                setType(newTree, TYPE_OPERATOR);
                addChild(newTree, mul2);
                replaceTree(mul1, newTree);
                replaceTree(node, node1);
                return true;
            }
        }
    return false;
}

bool substraction(Tree node) {
    return false;
}

bool multiplication(Tree node) {
    return false;
}

bool division(Tree node) {
    return false;
}

bool power(Tree node) {
    return false;
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
        value = getValue(expr);
        if (strcmp(value, ADDITION_SIGN) == 0) change = addition(expr);
        if (strcmp(value, SUBSTRACTION_SIGN) == 0) change = substraction(expr);
        if (strcmp(value, MULTIPLICATION_SIGN) == 0) change = multiplication(expr);
        if (strcmp(value, DIVISION_SIGN) == 0) change = division(expr);
        if (strcmp(value, POWER_SIGN) == 0) change = power(expr);
        if (debug) printTree(findRoot(expr));
        printf("%d\n", change);
        if (change) {
            expr = goToLeaf(expr);
            if (isEmptyTree(getParent(expr))) return expr;
            if (isEmptyTree(getParent(getParent(expr)))) expr = goToNextChild(expr);
            else expr = getParent(expr);
        }
        else expr = goToNextChild(expr);
    }
    return expr;
}