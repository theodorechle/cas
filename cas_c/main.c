#include "main.h"

String* getUserInput() {
    String* expr=createString();
    char c;
    while (true) {
        c = getchar();
        if (c == EOF || c == '\n') return expr;
        appendCharToString(expr, c);
    }
}

int main() {
    int debug;
    printf("Expression : ");
    String* expr=getUserInput();
    TreeList* tl, *tmp;
    Tree* tree;
    printf("1 for debug, 0 else : ");
    scanf("%d", &debug);
    tl = parser(expr->str, debug);
    if (debug) {
        tmp = tl;
        while (tmp != NULL) {
            printTree(tmp->tree);
            tmp = tmp->next;
        }
    }
    tree = parsedToTree(tl, debug);
    printf("Final tree\n");
    printTree(tree);
    printf("%s\n", treeStr(tree));
    return 0;
}