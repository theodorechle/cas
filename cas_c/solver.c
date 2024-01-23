#include "solver.h"

void sortTree(Tree t) {
    if (getType(t) != TYPE_OPERATOR) return;
    Tree child1 = getChild(t, 0);
    Tree child2 = getChild(t, 1);
    sortTree(child1);
    sortTree(child2);
    if (!strcmp(getValue(t), DIVISION_SIGN) || !strcmp(getValue(t), SUBSTRACTION_SIGN)) return;
    // sort actual tree
    int type1 = getType(child1), type2 = getType(child2);
    if ((type1 == TYPE_NUMBER && type2 == TYPE_VARIABLE && !strcmp(getValue(t), ADDITION_SIGN))
    || (type1 == TYPE_VARIABLE && type2 == TYPE_NUMBER && !strcmp(getValue(t), MULTIPLICATION_SIGN)))
        invertTree(child1, child2);
    if (type1 == type2 && (type1 == TYPE_VARIABLE || type1 == TYPE_OPERATOR) && strcmp(getValue(child1), getValue(child2)) > 0)
            invertTree(child1, child2);
    if (type1 != TYPE_OPERATOR && type2 == TYPE_OPERATOR)
        invertTree(child1, child2);
}

void findSuperiorGroup(Tree node, int priority, TreeList* nodes) {
    if (getType(node) != TYPE_OPERATOR || getPriority(getValue(node)) > priority) {
        nodes = addTree(nodes, node);
        return;
    }
    TreeList* child = node->childs;
    while (child != NULL) {
        findSuperiorGroup(child->tree, priority, nodes);
        child = child->next;
    }
}

bool additionable(Tree node1, Tree node2) {
    int type = getType(node1);
    char* value1, *value2;
    if (type != getType(node2) || type == TYPE_FUNCTION) return false;
    value1 = getValue(node1);
    value2 = getValue(node2);
    if (type == TYPE_VARIABLE && strcmp(value1, value2)) return false; // diferents variables
    if (type == TYPE_OPERATOR) {
        // childs are not the sames
        if (!additionable(getChild(node1, 0), getChild(node2, 0))
        || !additionable(getChild(node1, 1), getChild(node2, 1)))
            // an operator where childs' order is important 
            if ((!strcmp(value1, DIVISION_SIGN) || !strcmp(value1, SUBSTRACTION_SIGN)) && strcmp(value1, value2)) return false;
        if (!additionable(getChild(node1, 0), getChild(node2, 1))
        || !additionable(getChild(node1, 1), getChild(node2, 0)))
            return false;
    }
    return true;
}

bool addition(Tree node) {
    Tree child1 = getChild(node, 0);
    Tree child2 = getChild(node, 1);
    int typeN1 = getType(child1), typeN2 = getType(child2);
    Tree newTree;
    TreeList* group1, *group2;
    TreeList* node1, *node2;
    Tree mul1 = NULL, mul2 = NULL;
    bool areValid = false;
    char* tmpChar;
    if (typeN1 == typeN2 && typeN1 == TYPE_NUMBER) {
            tmpChar = (char*)malloc(sizeof(double));
            if (tmpChar == NULL) {
                fprintf(stderr, "error in 'addition', dynamic allocation of string failed\n");
                exit(1);
            }
            sprintf(tmpChar, "%f", atof(getValue(child1)) + atof(getValue(child2)));
            newTree = createTree();
            setValue(newTree, tmpChar);
            setType(newTree, TYPE_NUMBER);
            replaceTree(node, newTree);
            return true;
    }
    group1 = createTreeList();
    group2 = createTreeList();
    findSuperiorGroup(child1, ADDITION_PRIORITY, group1);
    findSuperiorGroup(child2, ADDITION_PRIORITY, group2);
    if (group1->tree == NULL || group2->tree == NULL) return false;
    if (getType(group1->tree) == TYPE_OPERATOR && getType(getChild(group1->tree, 0)) == TYPE_NUMBER) {
        mul1 = getChild(group1->tree, 0);
        group1->tree = getChild(group1->tree, 1);
    }
    if (getType(group2->tree) == TYPE_OPERATOR && getType(getChild(group2->tree, 0)) == TYPE_NUMBER) {
        mul2 = getChild(group2->tree, 0);
        group2->tree = getChild(group2->tree, 1);
    }
    node1 = group1;
    while (node1 != NULL) {
        node2 = group2;
        while (node2 != NULL) {
            areValid = additionable(node1->tree, node2->tree);
            if (areValid) break;
            node2 = node2->next;
        }
        if (areValid) break;
        node1 = node1->next;
    }
    if (!areValid) return false;
    printTreeList(node1);
    printTreeList(node2);
    // same type for both
    if (getType(node1->tree) == TYPE_NUMBER) {
        newTree = createTreeWithValues(ADDITION_SIGN, TYPE_OPERATOR);
        replaceTree(addEmptyChild(newTree), node1->tree);
        replaceTree(addEmptyChild(newTree), node2->tree);
        replaceTree(node1->tree, newTree);
        free(node1);
        // deleteTreeInList(getParent(node2->tree)->childs, node2->tree->childIndex);
        // deleteTreeList(node2);
        // printTree(node);
        // deleteTreeAndChilds(child2);
        // printTree(node);
        replaceTree(node, child1);
        return true;
    }
    if (isEmptyTree(mul1)) {
        newTree = createTreeWithValues(MULTIPLICATION_SIGN, TYPE_OPERATOR);
        mul1 = createTreeWithValues("1", TYPE_NUMBER);
        replaceTree(addEmptyChild(newTree), node1->tree);
        addChild(newTree, mul1);
        replaceTree(node1->tree, newTree);
    }
    if (isEmptyTree(mul2)) mul2 = createTreeWithValues("1", TYPE_NUMBER);
    
    newTree = createTreeWithValues(ADDITION_SIGN, TYPE_OPERATOR);
    replaceTree(addEmptyChild(newTree), mul1);
    replaceTree(addEmptyChild(newTree), mul2);
    replaceTree(mul1, newTree);
    deleteTreeAndChilds(child2);
    replaceTree(node, child1);
    return true;
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

Tree goToNextExpr(Tree node, bool* change) {
    if (isEmptyTree(node) || isEmptyTree(getParent(node))) {
        *change = false;
        return node;
    }
    *change = true;
    if (getNbChilds(getParent(node)) - 1 > node->childIndex) return goToLeaf(getChild(getParent(node), node->childIndex + 1));
    return getParent(node);
}

Tree solve(Tree expr, bool debug) {
    /**
     * Go to the parent of the first leaf, solve it, if not modified, solve it's parent, else solve his first leaf's parent
    */
    bool change = true;
    char* value;
    Tree newExpr;
    sortTree(expr);
    expr = getParent(goToLeaf(expr)); // go to first expression to evaluate : the father of the first leaf, because the leaf itself can't be evaluated
    while (!isEmptyTree(getParent(expr)) || change == true) {
        value = getValue(expr);
        if (getType(expr) == TYPE_OPERATOR) {
            if (strcmp(value, ADDITION_SIGN) == 0) change = addition(expr);
            if (strcmp(value, SUBSTRACTION_SIGN) == 0) change = substraction(expr);
            if (strcmp(value, MULTIPLICATION_SIGN) == 0) change = multiplication(expr);
            if (strcmp(value, DIVISION_SIGN) == 0) change = division(expr);
            if (strcmp(value, POWER_SIGN) == 0) change = power(expr);
        }
        else change = false;
        if (debug) {
            printf("entire expression\n");
            printTree(findRoot(expr));
            printf("\nactual tree\n");
            printTree(expr);
            printf("changed : %d\n", change);
        }
        if (change) {
            newExpr = goToLeaf(expr);
            if (isEmptyTree(getParent(newExpr))) {
                expr = newExpr;
                break;
            }
            if (getParent(newExpr) == expr) expr = goToNextExpr(expr, &change);
            expr = newExpr;
        }
        else expr = goToNextExpr(expr, &change);
        sortTree(expr);
    }
    return expr;
}