#include "main.h"

int main() {
    Tree* t=createTree();
    setValue(t, "+");
    setType(t, 0);
    t = addChild(t, createTree());
    setValue(t, "5");
    setType(t, 1);
    t = getParent(t);
    t = addChild(t, createTree());
    setValue(t, "7");
    setType(t, 1);
    t = getParent(t);
    printf("%s (%d)\n", getValue(t), getType(t));
    for (int i=0; i < getNbChilds(t); i++) {
        printf("\t%s (%d)\n", getValue(getChild(t, i)), getType(getChild(t, i)));
    }
    return 0;
}