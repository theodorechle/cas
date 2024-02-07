#include "tree.h"

TreeList* createTreeList() {
    TreeList* t;
    t = (TreeList*)malloc(sizeof(TreeList));
    if (t == NULL) {
        fprintf(stderr, "error in 'createTreeList', dynamic allocation failed\n");
        exit(1);
    }
    t->tree = NULL;
    t->next = NULL;
    return t;
}

char* createString() {
    char* str;
    str = (char*)malloc(sizeof(char));
    if (str == NULL) {
        fprintf(stderr, "error in 'createString', dynamic allocation failed\n");
        exit(1);
    }
    str[0] = '\0';
    return str;
}

Tree createTree() {
    Tree t;
    t = (Tree)malloc(sizeof(Node));
    if (t == NULL) {
        fprintf(stderr, "error in 'createTree', dynamic allocation failed\n");
        exit(1);
    }
    t->value = createString();
    t->type = 0;
    t->parent = NULL;
    t->childIndex = 0;
    t->childs = NULL;
    return t;
}

Tree createTreeWithValues(char* value, int type) {
    Tree t = createTree();
    setValue(t, value);
    setType(t, type);
    return t;
}

TreeList* addTree(TreeList* trees, Tree tree) {
    if (trees == NULL)
        trees = createTreeList();
    if (trees->tree == NULL) {
        trees->tree = tree;
        return trees;
    }
    trees->next = addTree(trees->next, tree);
    return trees;
}

Tree addChild(Tree t, Tree child) {
    Tree newTree;
    if (t == NULL) {
        fprintf(stderr, "error : 'addChild' : tree is NULL\n");
        exit(4);
    }
    t->childs = addTree(t->childs, child);
    newTree = getChild(t, -1);
    newTree->childIndex = getNbChilds(t)-1;
    setParent(newTree, t);
    return newTree;
}

Tree addEmptyChild(Tree t) {
    return addChild(t, createTree());
}

void deleteTreeList(TreeList* tl) {
    TreeList* next=tl;
    while (tl != NULL) {
        next = next->next;
        deleteTreeAndChilds(tl->tree);
        free(tl);
        tl = next;
    }

}

Tree deleteChilds(Tree t) {
    if (t->childs == NULL) return NULL;
    deleteTreeList(t->childs);
    return t;
}

void deleteTree(Tree t) {
    free(t);
}

void deleteTreeAndChilds(Tree t) {
    if (t == NULL) return;
    free(t->value);
    deleteChilds(t);
    deleteTree(t);
}

char* appendToString(char* value, char* str) {
    int length = strlen(value) + strlen(str) + 1;
    value = (char*)realloc(value, length * sizeof(char));
    if (value == NULL) {
        fprintf(stderr, "error : 'appendToValue' : reallocation failed\n");
        exit(4);
    }
    strcat(value, str);
    return value;
}

Tree appendToValue(Tree t, char* str) {
    t->value = appendToString(t->value, str);
    return t;
}

char* appendCharToString(char* value, char character) {
    char c[2];
    c[0] = character;
    value = appendToString(value, c);
    return value;
}

char* clearString(char* str) {
    str = (char*)realloc(str, sizeof(char));
    if (str == NULL) {
        fprintf(stderr, "error : 'appendToValue' : reallocation failed\n");
        exit(4);
    }
    str[0] = '\0';
    return str;
}

Tree clearValue(Tree t) {
    t->value = clearString(t->value);
    return t;
}

Tree setValue(Tree t, char* value) {
    clearValue(t);
    appendToValue(t, value);
    return t;
}

Tree setType(Tree t, int type) {
    t->type=type;
    return t;
}

Tree setParent(Tree t, Tree p) {
    t->parent=p;
    return t;
}

char* getValue(Tree t) {
    return t->value;
}

int getType(Tree t) {
    return t->type;
}

Tree getParent(Tree t) {
    return t->parent;
}

int getNbTrees(TreeList* tl) {
    int nb=0;
    while (tl != NULL) {
        nb++;
        tl = tl->next;
    }
    return nb;
}

int getNbChilds(Tree t) {
    return getNbTrees(t->childs);
}

Tree getNextChild(Tree t) {
    // if (getNbChilds(getParent(t)) - 1 > t->childIndex) return goToLeaf(getChild(getParent(t), t->childIndex + 1));
    return NULL;
}

Tree getTree(TreeList* tl, int nb) {
    int l = getNbTrees(tl);
    if (nb < 0)
        nb += l;
    if (nb >= l) {
        fprintf(stderr, "error : 'getTree' : trying to access child %d of %d\n", nb, l);
        exit(2);
    }
    for (nb; nb > 0; nb--) {
        tl = tl->next;
    }
    return tl->tree;
}
Tree getChild(Tree t, int nb) {
    return getTree(t->childs, nb);
}

Tree replaceTree(Tree tree1, Tree tree2) {
    tree1->value = tree2->value;
    tree1->type = tree2->type;
    tree1->childs = tree2->childs;
    tree2->childs = createTreeList();
    for (int i=0; i<getNbChilds(tree1); i++)
        getChild(tree1, i)->parent = tree1;
    return tree1;
}

void invertTree(Tree tree1, Tree tree2) {
    Tree tmp = replaceTree(createTree(), tree1);
    replaceTree(tree1, tree2);
    replaceTree(tree2, tmp);
    deleteTree(tmp);
}

Tree copyTree(Tree tree) {
    Tree newTree = createTree();
    int l=getNbChilds(tree);
    setValue(newTree, getValue(tree));
    setType(newTree, getType(tree));
    for (int i=0; i<l; i++)
        addChild(newTree, copyTree(getChild(tree, i)));
    return newTree;
}

void __printTree(Tree tree, int level) {
    if (isEmptyTree(tree)) return;
    for (int i=0; i<level; i++)
        printf("\t");
    printf("%s (%s)\n", tree->value, TYPES[tree->type]);
    for (int i=0; i<getNbChilds(tree); i++)
        __printTree(getChild(tree, i), level+1);
}

void printTree(Tree tree) {
    __printTree(tree, 0);
}

void printTreeList(TreeList* tl) {
    printf("[ ");
    while (tl != NULL && !isEmptyTree(tl->tree)) {
        printf("%s (%s), ", tl->tree->value, TYPES[tl->tree->type]);
        tl = tl->next;
    }
    printf("]\n");
}

Tree findRoot(Tree tree) {
    if (isEmptyTree(tree)) return tree;
    while (!isEmptyTree(getParent(tree))) tree = getParent(tree);
    return tree;
}

int treeLength(Tree tree) {
    if (tree == NULL) return 0;
    int nb=0, n;
    for (int i=0; i<getNbChilds(tree); i++) {
        n = treeLength(getChild(tree, i));
        if (n > nb)
            nb = n;
    }
    return nb + 1;
}

char* treeStr(Tree tree) {
    if (isEmptyTree(tree)) return "";
    bool parenthesis;
    char* str = createString();
    if (getType(tree) == TYPE_OPERATOR) {
        // add parenthesis if father is operator and have bigger priority
        if (getParent(tree) != NULL && getType(getParent(tree)) == TYPE_OPERATOR && getPriority(getValue(tree)) < getPriority(getValue(getParent(tree)))) {
            parenthesis = true;
            str = appendToString(str, "(");
        }
        str = appendToString(str, treeStr(getChild(tree, 0)));
        str = appendToString(str, getValue(tree));
        str = appendToString(str, treeStr(getChild(tree, 1)));
        if (parenthesis) str = appendToString(str, ")");
    }
    else if (tree->type == TYPE_FUNCTION)
    {
        str = appendToString(str, getValue(tree));
        str = appendToString(str, "(");
        if (getNbChilds(tree)) {
            str = appendToString(str, treeStr(getChild(tree, 0)));
            for (int i=1; i<getNbChilds(tree); i++) {
                str = appendToString(str, ", ");
                str = appendToString(str, treeStr(getChild(tree, i)));
            }
        }
        str = appendToString(str, ")");
    }
    else str = appendToString(str, getValue(tree));
    return str;
}

int getPriority(char* operator) {
    if (!strcmp(operator, ADDITION_SIGN)) return ADDITION_PRIORITY;
    if (!strcmp(operator, SUBSTRACTION_SIGN)) return SUBSTRACTION_PRIORITY;
    if (!strcmp(operator, MULTIPLICATION_SIGN)) return MULTIPLICATION_PRIORITY;
    if (!strcmp(operator, DIVISION_SIGN)) return DIVISION_PRIORITY;
    if (!strcmp(operator, POWER_SIGN)) return POWER_PRIORITY;
    if (!strcmp(operator, IMPLICIT_MULTIPLICATION_SIGN)) return IMPLICIT_MULTIPLICATION_PRIORITY;
    fprintf(stderr, "Error in priority : '%s' is not an operator\n", operator);
}

bool isEmptyValue(char* str) {
    return str[0] == '\0';
}

bool isEmptyTree(Tree tree) {
    return tree == NULL;
}

TreeList* deleteTreeInList(TreeList* tl, int index) {
    int l = getNbTrees(tl);
    if (index < 0)
        index += l;
    if (index >= l) {
        fprintf(stderr, "error : 'deleteTreeInList' : trying to access child %d of %d\n", index, l);
        exit(2);
    }
    if (index == 0) return tl->next;
    for (int i=0; i < index-1; i++) {
        tl = tl->next;
    }
    tl->next = tl->next->next;
    return tl;
}
