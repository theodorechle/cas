#include "main.h"

char *getUserInput() {
    string expr{""};
    char c;
    while (true) {
        c = getchar();
        if (c == EOF || c == '\n') return expr;
        expr = appendCharToString(expr, c);
    }
}

int main() {
    int debug, implicitPriority;
    printf("Expression : ");
    char *expr = getUserInput();
    TreeList *tl, *tmp;
    Tree tree;
    printf("1 for debug, 0 else : ");
    scanf("%d", &debug);
    printf("1 for implicit priority, 0 else : ");
    scanf("%d", &implicitPriority);
    tl = parser(expr, debug, implicitPriority);
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
    printf("Start expression :\n%s\n", expr);
    printf("Final expression :\n%s\n", treeStr(tree));
    return 0;
}