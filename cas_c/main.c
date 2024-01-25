#include "main.h"

String getUserInput() {
    String expr=createString();
    char c;
    while (true) {
        c = getchar();
        if (c == EOF || c == '\n') return expr;
        appendCharToString(expr, c);
    }
}

int main() {
    int debug, implicitPriority;
    printf("Expression : ");
    String expr=getUserInput();
    TreeList* tl, *tmp;
    Tree tree;
    printf("1 for debug, 0 else : ");
    scanf("%d", &debug);
    printf("1 for implicit priority, 0 else : ");
    scanf("%d", &implicitPriority);
    tl = parser(expr->str, debug, implicitPriority);
    if (debug) {
        tmp = tl;
        while (tmp != NULL) {
            printTree(tmp->tree);
            tmp = tmp->next;
        }
    }
    tree = parsedToTree(tl, debug, implicitPriority);
    if (debug) {
        printf("Complete tree\n");
        printTree(tree);
        printf("%s\n", treeStr(tree));
    }    
    tree = solve(tree, debug);
    if (debug) {
        printf("Solved tree\n");
        printTree(tree);
    }
    printf("Start expression :\n%s\n", expr->str);
    printf("Final expression :\n%s\n", treeStr(tree));
    return 0;
}