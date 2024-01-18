#include "tree.h"

TreeList* createTreeList() {
    TreeList* t;
    t = (TreeList*)malloc(sizeof(TreeList));
    if (t == NULL) {
        fprintf(stderr, "error : 'createChild' : erreur malloc\n");
        exit(1);
    }
    t->tree = NULL;
    t->next = NULL;
    return t;
}

String createString() {
    String s;
    char* str;
    int size=5;
    s = (String)malloc(sizeof(String));
    str = (char*)malloc(sizeof(char) * size);
    if (s == NULL || str == NULL) {
        fprintf(stderr, "error : 'createString' : erreur malloc");
        exit(1);
    }
    s->size = size;
    s->str = str;
    return s;
}

Tree createTree() {
    Tree t;
    t = (Tree)malloc(sizeof(Node));
    if (t == NULL) {
        fprintf(stderr, "error : 'createTree' : erreur malloc\n");
        exit(1);
    }
    t->value = createString();
    t->type = 0;
    t->parent = NULL;
    t->childIndex = 0;
    t->childs = NULL;
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
        deleteTree(tl->tree);
        free(tl);
        tl = next;
    }

}

Tree deleteChilds(Tree t) {
    if (t->childs == NULL) return NULL;
    deleteTreeList(t->childs);
    return t;
}

Tree deleteTree(Tree t) {
    if (t == NULL) return NULL;
    free(t->value);
    deleteChilds(t);
    free(t);
    return NULL;
}

String appendToString(String value, char* str) {
    int length=strlen(value->str) + strlen(str) + 1;
    if (length >= value->size) {
        value->size = length;
        value->str = (char*)realloc(value->str, (value->size) * sizeof(char));
        if (value->str == NULL) {
            fprintf(stderr, "error : 'appendToValue' : erreur realloc\n");
            exit(4);
        }
    }
    strcat(value->str, str);
    return value;
}

Tree appendToValue(Tree t, char* str) {
    appendToString(t->value, str);
    return t;
}

String appendCharToString(String value, char character) {
    char c[2];
    c[0] = character;
    appendToString(value, c);
    return value;
}

String clearString(String str) {
    str->str[0] = '\0';
    return str;
}

Tree clearValue(Tree t) {
    clearString(t->value);
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
    return t->value->str;
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
    for (int i=0; i<getNbChilds(tree1); i++)
        getChild(tree1, i)->parent = tree1;
    return tree1;
}

void __printTree(Tree tree, int level) {
    for (int i=0; i<level; i++)
        printf("\t");
    printf("%s (%s)\n", tree->value->str, TYPES[tree->type]);
    for (int i=0; i<getNbChilds(tree); i++)
        __printTree(getChild(tree, i), level+1);
}

void printTree(Tree tree) {
    __printTree(tree, 0);
}

void printTreeList(TreeList* tl) {
    printf("[ ");
    while (tl != NULL && !isEmptyTree(tl->tree)) {
        printf("%s (%s), ", tl->tree->value->str, TYPES[tl->tree->type]);
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
    bool parenthesis;
    String str;
    str = createString();
    if (getType(tree) == TYPE_OPERATOR) {
        if (getParent(tree) != NULL && getType(getParent(tree)) == TYPE_OPERATOR && getPriority(getValue(tree)) < getPriority(getValue(getParent(tree)))) {
            parenthesis = true;
            appendToString(str, "(");
        }
        appendToString(str, treeStr(getChild(tree, 0)));
        appendToString(str, getValue(tree));
        appendToString(str, treeStr(getChild(tree, 1)));
        if (parenthesis)
            appendToString(str, ")");
    }
    else if (tree->type == TYPE_FUNCTION)
    {
        appendToString(str, getValue(tree));
        appendToString(str, "(");
        if (getNbChilds(tree)) {
            appendToString(str, treeStr(getChild(tree, 0)));
            for (int i=1; i<getNbChilds(tree); i++) {
                appendToString(str, ", ");
                appendToString(str, treeStr(getChild(tree, i)));
            }
        }
        appendToString(str, ")");
    }
    else
        appendToString(str, getValue(tree));
    return str->str;
}

int getPriority(char* operator) {
    if (!strcmp(operator, ADDITION_SIGN)) return ADDITION_PRIORITY;
    if (!strcmp(operator, SUBSTRACTION_SIGN)) return SUBSTRACTION_PRIORITY;
    if (!strcmp(operator, MULTIPLICATION_SIGN)) return MULTIPLICATION_PRIORITY;
    if (!strcmp(operator, DIVISION_SIGN)) return DIVISION_PRIORITY;
    if (!strcmp(operator, POWER_SIGN)) return POWER_PRIORITY;
    if (!strcmp(operator, IMPLICIT_MULTIPLICATION_SIGN)) return IMPLICIT_MULTIPLICATION_PRIORITY;
    fprintf(stderr, "Error in priority : '%s' is not an operator\n");
}

bool isEmptyValue(String str) {
    return str->str[0] == '\0';
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