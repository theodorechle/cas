#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "constants.h"

struct treeList;



/**
  *value : The tree value
  *type : The type of the tree, like TYPE_OPERATOR, TYPE_INT, ...
  *parent : The parent of the tree, who itself will reference the tree in his childs
  *childIndex : The index where the tree is in the childs' list of his father
*/
typedef struct tree {
    char *value;
    int type;
    struct tree *parent;
    int childIndex;
    struct treeList *childs;
} Node, *Tree;

typedef struct treeList {
    Tree tree;
    struct treeList *next;
} TreeList;


TreeList *createTreeList();

char *createString();

Tree createTree();

Tree createTreeWithValues(char *value, int type);

/**
  *replace the value, the type and the childs of tree1 with the tree2 ones
  *set tree1 the parent of his childs
  *@return tree1
*/
Tree replaceTree(Tree tree1, Tree tree2);

void invertTree(Tree tree1, Tree tree2);

Tree copyTree(Tree tree);

TreeList *addTree(TreeList *trees, Tree tree);

/**
  *Add the child at the end of the tree childs' list
  *Change child's childIndex
  *@return The new child
*/
Tree addChild(Tree t, Tree child);

Tree addEmptyChild(Tree t);

void deleteTreeList(TreeList *tl);

Tree deleteChilds(Tree t);

void deleteTree(Tree t);

void deleteTreeAndChilds(Tree t);

char *appendToString(char *value, char *str);

Tree appendToValue(Tree t, char *str);

char *appendCharToString(char *value, char character);

char *clearString(char *str);

Tree clearValue(Tree t);

Tree setValue(Tree t, char *value);

Tree setType(Tree t, int type);

Tree setParent(Tree t, Tree p);

char *getValue(Tree t);

int getType(Tree t);

Tree getParent(Tree t);

int getNbTrees(TreeList *tl);

int getNbChilds(Tree t);

Tree getChild(Tree t, int nb);

Tree getNextChild(Tree t);

void __printTree(Tree tree, int level);

void printTree(Tree tree);

void printTreeList(TreeList *tl);

Tree findRoot(Tree tree);

int treeLength(Tree tree);

char *treeStr(Tree tree);

int getPriority(char *operator);

bool isEmptyValue(char *str);

bool isEmptyTree(Tree tree);

TreeList *deleteTreeInList(TreeList *tl, int index);