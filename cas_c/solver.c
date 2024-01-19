#include "solver.h"

bool checkSameVars(TreeList* list, TreeList* list2) {
    char* value;
    bool find = false;
    int i;
    TreeList* tmp;
    while (list != NULL) {
        if (isEmptyTree(list->tree)) return false;
        value = getValue(list->tree);
        find = false;
        i = 0;
        tmp = list2;
        while (tmp != NULL && !isEmptyTree(tmp->tree)) {
            if (getType(tmp->tree) == TYPE_VARIABLE && strcmp(getValue(tmp->tree), value) == 0) {
                tmp = deleteTreeInList(tmp, i);
                find = true;
                break;
            }
            i++;
            tmp = tmp->next;
        }
        if (!find) return false;
        list = list->next;
    }
    return true;
}

Tree getMultiplicator(Tree t) {
    if (getType(t) == TYPE_NUMBER) return t;
    TreeList* child = t->childs;
    Tree c;
    while (child != NULL) {
        c = getMultiplicator(child->tree);
        if (!isEmptyTree(c)) return c;
        child = child->next;
    }
    return NULL;
}

void getVars(Tree t, TreeList* vars) {
    if (getType(t) == TYPE_VARIABLE) vars = addTree(vars, t);
    TreeList* childs = t->childs;
    while (childs != NULL) {
        getVars(childs->tree, vars);
        childs = childs->next;
    }
}


void sortTree(Tree t) {
    if (getType(t) != TYPE_OPERATOR) return;
    Tree child1 = getChild(t, 0), child2 = getChild(t, 1);
    int type1 = getType(child1), type2 = getType(child2);
    if ((type1 == TYPE_NUMBER && type2 == TYPE_VARIABLE && !strcmp(getValue(t), ADDITION_SIGN))
    || (type1 == TYPE_VARIABLE && type2 == TYPE_NUMBER && !strcmp(getValue(t), MULTIPLICATION_SIGN)))
        invertTree(child1, child2);
    if (type1 == type2 && (type1 == TYPE_VARIABLE || type1 == TYPE_OPERATOR) && strcmp(getValue(child1), getValue(child2)) > 0)
            invertTree(child1, child2);
    if (type1 != TYPE_OPERATOR && type2 == TYPE_OPERATOR)
        invertTree(child1, child2);
    sortTree(child1);
    sortTree(child2);
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

bool getCompatible(TreeList* nodes1, TreeList* nodes2, Tree* t1, Tree* t2) {
    TreeList* n, *vars1, *vars2;
    while (nodes1 != NULL) {
        n = nodes2;
        vars1 = createTreeList();
        getVars(nodes1->tree, vars1);
        while (n != NULL) {
            vars2 = createTreeList();
            getVars(n->tree, vars2);
            if (checkSameVars(vars1, vars2)) {
                *t1 = nodes1->tree;
                *t2 = n->tree;
                return true;
            }
            // deleteTreeList(vars2);
            n = n->next;
        }
        // deleteTreeList(vars1);
        nodes1 = nodes1->next;
    }
    return false;
}

bool addition(Tree node) {
    Tree node1 = getChild(node, 0);
    Tree node2 = getChild(node, 1);
    Tree sup1, sup2;
    int typeN1 = getType(node1), typeN2 = getType(node2);
    Tree newTree = createTree();
    TreeList* vars1 = createTreeList(), *vars2 = createTreeList(), *nodes1=createTreeList(), *nodes2=createTreeList();
    Tree mul1, mul2, child;
    char* s = (char*)malloc(sizeof(double));
    if (typeN1 == typeN2 && typeN1 == TYPE_NUMBER) {
            sprintf(s, "%f", atof(getValue(node1)) + atof(getValue(node2)));
            setValue(newTree, s);
            setType(newTree, TYPE_NUMBER);
            replaceTree(node, newTree);
            return true;
    }
    findSuperiorGroup(node1, ADDITION_PRIORITY, nodes1);
    findSuperiorGroup(node2, ADDITION_PRIORITY, nodes2);
    if (!getCompatible(nodes1, nodes2, &sup1, &sup2)) return false;
    mul1 = getMultiplicator(sup1);
    mul2 = getMultiplicator(sup2);
    // if no multiplicator, set it to 1
    // example : a*b => 1*a*b
    if (isEmptyTree(mul1)) {
        mul1 = setType(setValue(createTree(), "1"), TYPE_NUMBER);
        replaceTree(addEmptyChild(newTree), sup1);
        addChild(newTree, mul1);
        setType(newTree, TYPE_OPERATOR);
        setValue(newTree, MULTIPLICATION_SIGN);
        replaceTree(sup1, newTree);
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
    if (treeLength(expr) == 1) return expr;
    expr = getParent(goToLeaf(expr)); // go to first expression to evaluate : the father of the first leaf, because the leaf itself can't be evaluated
    while (!isEmptyTree(getParent(expr)) || change == true) {
        sortTree(expr);
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
            if (isEmptyTree(getParent(newExpr))) return newExpr;
            if (getParent(newExpr) == expr) expr = goToNextExpr(expr, &change);
            expr = newExpr;
        }
        else expr = goToNextExpr(expr, &change);
    }
    return expr;
}