#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "constants.h"

typedef struct {
    char* str;
    int size;
} String;

struct treeList;

/**
 * value : The tree value
 * type : The type of the tree, like TYPE_OPERATOR, TYPE_INT, ...
 * parent : The parent of the tree, who itself will reference the tree in his childs
 * childIndex : The index where the tree is in the childs' list of his father
*/
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

Tree* replaceTree(Tree* tree1, Tree* tree2);

/**
 * Add the child at the end of the tree childs' list
 * Doesn't change child
*/
Tree* addChild(Tree* t, Tree* tree);

Tree* deleteChilds(Tree* t);

Tree* deleteTree(Tree* t);

void appendToString(String* value, char* str);

void appendToValue(Tree* t, char* str);

void setValue(Tree* t, char* value);

void setType(Tree* t, int type);

void setParent(Tree* t, Tree* p);

char* getValue(Tree* t);

int getType(Tree* t);

Tree* getParent(Tree* t);

int getNbChilds(Tree* t);

Tree* getChild(Tree* t, int nb);

void printTree(Tree* tree, int level);

Tree* findRoot(Tree* tree);

int treeLength(Tree* tree);

char* treeStr(Tree* tree);