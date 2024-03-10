#include <string>
#include <iostream>

#include "node.hpp"
#include "parser.hpp"
#include "solver.hpp"

using namespace std;

int main() {
    bool debug, implicitPriority;
    string expr;
    Node *tl, *tree;
    cout << "Expression : ";
    getline(cin, expr);
    cout << "1 for debug, 0 else : ";
    cin >> debug;
    cout << "1 for implicit priority, 0 else : ";
    cin >> implicitPriority;
    tl = parser(expr, debug, implicitPriority);
    if (debug) {
        Node *tmp = tl;
        while (tmp != NULL) {
            tmp->display(cerr);
            tmp = tmp->getNext();
        }
    }
    tree = parsedToTree(tl, debug, implicitPriority);
    delete tl;
    if (debug) {
        cerr << "Parsed tree" << endl;
        tree->display(cerr);
        cerr << tree->str() << endl;
    }
    // tree = solve(tree, debug);
    // if (debug) {
    //     cerr << "Solved tree" << endl);
    //     printTree(tree);
    // }
    // cout << "Start expression : " << endl << expr << endl;
    // cout << "Final expression : " << endl << treeStr(tree) < endl;
    delete tree;
    return 0;
}