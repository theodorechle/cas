#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct {
    char* str;
    int size;
} String;

struct treeList;

typedef struct tree {
    String* value;
    int type;
    struct tree* parent;
    int childIndex;
    struct treeList* childs;
} Tree;

typedef struct treeList {
    Tree* tree;
    struct treeList* next;
} TreeList;

TreeList* createTreeList();
String* createString();
Tree* createTree();
Tree* addChild(Tree* t, Tree* tree);
Tree* deleteChilds(Tree* t);
Tree* deleteTree(Tree* t);
void appendToValue(Tree* t, char* str);
void setValue(Tree* t, char* value);
void setType(Tree* t, int type);
void setParent(Tree* t, Tree* p);
char* getValue(Tree* t);
int getType(Tree* t);
Tree* getParent(Tree* t);
int getNbChilds(Tree* t);
Tree* getChild(Tree* t, int nb);