Working on addition with litterals and non-litterals.

TODO : find a way to free the TreeLists create in getCompatible
allow addition between number and number in operation (ex : 3+a+5)

use findSuperiorGroup, then sort the trees (addition : a+5, multiplication : 5*a, alphabetic order, non operators after operators, don't touch division and substraction and power, sort operators with their values)
If same values for two operators, when checking if same trees, explore boths childs.
When checking if same trees, allow different numbers for first level of multiplication and set it to the multiplicator, not for others levels.


c(a+2b)+c(a+b) // don't solve before multiplication solve (next step)
ac + 2bc + ac + bc
2ac+3bc

c(a+2b)+c(a+2b) // solve
2c(a+2b)
2ac+4bc

ab + c * d + abc

*
    c
    +
        a
        *
            2
            b

Remove childIndex


typedef struct {
    Node *child;
    Node *next;
    Node *parent;
    char *value;
    int type;
} Node;


-71a+4b-4a*b+7+12a-7