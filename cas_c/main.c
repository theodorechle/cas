#include "main.h"

int main() {
    Tree* tree=createTree();
    setValue(tree, "+");
    setType(tree, TYPE_OPERATOR);
    tree = addChild(tree, createTree());
    setValue(tree, "5");
    appendToValue(tree, "7");
    setType(tree, TYPE_NUMBER);
    tree = getParent(tree);
    tree = addChild(tree, createTree());
    setValue(tree, "7");
    setType(tree, TYPE_NUMBER);
    tree = getParent(tree);
    printTree(tree, 0);
    printf("size : %d\n", treeLength(tree));
    printf("%s\n", treeStr(tree));
    return 0;
}