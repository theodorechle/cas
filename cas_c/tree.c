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

String* createString() {
    String* s;
    char* str;
    int size=5;
    s = (String*)malloc(sizeof(String));
    str = (char*)malloc(sizeof(char) * size);
    if (s == NULL || str == NULL) {
        fprintf(stderr, "error : 'createString' : erreur malloc");
        exit(1);
    }
    s->size = size;
    s->str = str;
    return s;
}

Tree* createTree() {
    Tree* t;
    t = (Tree*)malloc(sizeof(Tree));
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

TreeList* addTree(TreeList* trees, Tree* tree) {
    TreeList* tl2=trees;
    Tree* newTree=createTree();
    newTree = replaceTree(newTree, tree);
    newTree->childIndex = getNbTrees(trees);
    if (trees == NULL) {
        trees = createTreeList();
        tl2 = trees;
    }
    else {
        while (tl2->next != NULL)
            tl2 = tl2->next;
        tl2->next = createTreeList();
        tl2 = tl2->next;
    }
    tl2->tree = newTree;
    return trees;
}

Tree* addChild(Tree* t, Tree* tree) {
    Tree* newTree;
    if (t == NULL) {
        fprintf(stderr, "error : 'addChild' : tree is NULL\n");
        exit(4);
    }
    t->childs = addTree(t->childs, tree);
    newTree = getChild(t, -1);
    setParent(newTree, t);
    return newTree;
}

Tree* deleteChilds(Tree* t) {
    if (t->childs == NULL) return NULL;
    TreeList* list=t->childs,*next=t->childs->next;
    while (t != NULL) {
        deleteTree(list->tree);
        free(list);
        list = next;
        next = list->next;
    }
    return t;
}

Tree* deleteTree(Tree* t) {
    free(t->value);
    deleteChilds(t);
    free(t);
    return NULL;
}

void appendToString(String* value, char* str) {
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
}

void appendToValue(Tree* t, char* str) {
    appendToString(t->value, str);
}

void appendCharToString(String* value, char character) {
    char c[2];
    c[0] = character;
    appendToString(value, c);
}

void clearString(String* str) {
    str->str[0] = '\0';
}

void clearValue(Tree* t) {
    clearString(t->value);
}

void setValue(Tree* t, char* value) {
    t->value->str[0]='\0';
    appendToValue(t, value);
}

void setType(Tree* t, int type) {
    t->type=type;
}

void setParent(Tree* t, Tree* p) {
    t->parent=p;
}

char* getValue(Tree* t) {
    return t->value->str;
}

int getType(Tree* t) {
    return t->type;
}

Tree* getParent(Tree* t) {
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

int getNbChilds(Tree* t) {
    return getNbTrees(t->childs);
}

Tree* getTree(TreeList* tl, int nb) {
    int l = getNbTrees(tl);
    if (nb < 0)
        nb = l + nb;
    if (nb >= l) {
        fprintf(stderr, "error : 'getTree' : trying to access child %d of %d\n", nb, l);
        exit(2);
    }
    for (nb; nb > 0; nb--) {
        tl = tl->next;
    }
    return tl->tree;
}
Tree* getChild(Tree* t, int nb) {
    return getTree(t->childs, nb);
}

Tree* replaceTree(Tree* tree1, Tree* tree2) {
    tree1->value = tree2->value;
    tree1->type = tree2->type;
    tree1->childs = tree2->childs;
    for (int i=0; i<getNbChilds(tree1); i++)
        getChild(tree1, i)->parent = tree1;
    return tree1;
}

void printTree(Tree* tree, int level) {
    for (int i=0; i<level; i++)
        printf("\t");
    printf("%s (%s)\n", tree->value->str, TYPES[tree->type]);
    for (int i=0; i<getNbChilds(tree); i++)
        printTree(getChild(tree, i), level+1);
}

Tree* findRoot(Tree* tree) {
    if (tree->parent == NULL)
        return tree;
    return findRoot(tree->parent);
}

int treeLength(Tree* tree) {
    if (tree == NULL) return 0;
    int nb=0, n;
    for (int i=0; i<getNbChilds(tree); i++) {
        n = treeLength(getChild(tree, i));
        if (n > nb)
            nb = n;
    }
    return nb + 1;
}

char* treeStr(Tree* tree) {
    bool parenthesis;
    String* str;
    str = createString();
    if (tree->type == TYPE_OPERATOR) {
        if (tree->parent != NULL && tree->type < tree->parent->type) {
            parenthesis = true;
            appendToString(str, "(");
        }
        appendToString(str, treeStr(getChild(tree, 0)));
        appendToString(str, tree->value->str);
        appendToString(str, treeStr(getChild(tree, 1)));
        if (parenthesis)
            appendToString(str, ")");
    }
    else if (tree->type == TYPE_FUNCTION)
    {
        appendToString(str, tree->value->str);
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
        appendToString(str, tree->value->str);
    return str->str;
}

bool isEmptyValue(String* str) {
    return !str->size;
}