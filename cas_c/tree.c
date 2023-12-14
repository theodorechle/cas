#include "tree.h"

TreeList* createTreeList() {
    TreeList* t;
    t = (TreeList*)malloc(sizeof(TreeList));
    if (t == NULL) {
        fprintf(stderr, "error : 'createTreeList' : erreur malloc\n");
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

Tree* addChild(Tree* t, Tree* tree) {
    TreeList* tl=t->childs;
    TreeList* tl2 = tl;
    tree->childIndex = getNbChilds(t);
    if (tl == NULL) {
        t->childs = createTreeList();
        tl2 = t->childs;
    }
    else {
        while (tl2->next != NULL)
            tl2 = tl2->next;
        tl2->next = createTreeList();
        tl2 = tl2->next;
    }
    setParent(tree, t);
    tl2->tree = tree;
    return tree;
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

void appendToValue(Tree* t, char* str) {
    String* value=t->value;
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

int getNbChilds(Tree* t) {
    int nb=0;
    TreeList* tl=t->childs;
    while (tl != NULL) {
        nb++;
        tl = tl->next;
    }
    return nb;
}

Tree* getChild(Tree* t, int nb) {
    TreeList* tl=t->childs;
    int l = getNbChilds(t);
    if (nb >= l) {
        fprintf(stderr, "error : 'getChilds' : trying to access child %d of %d\n", nb, l);
        exit(2);
    }
    for (nb; nb > 0; nb--) {
        tl = tl->next;
    }
    return tl->tree;
}