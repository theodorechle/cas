#include <string>
#include <iostream>

#include "main.hpp"

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
            tmp->display();
            tmp = tmp->getNext();
        }
    }
    tree = parsedToTree(tl, debug, implicitPriority);
    cout << "tree " << tree << endl;
    cout << "tree child 1 " << tree->getChild() << endl;
    cout << "tree child 2 " << tree->getChild()->getNext() << endl;
    cout << "tree list " << tl << endl;
    cout << "tree list 2 " << tl->getNext() << endl;
    cout << "tree list 3 " << tl->getNext()->getNext() << endl;
    delete tl;
    cout << "-------------------------" << endl;
    cout << "tree " << tree << endl;
    cout << "tree child 1 " << tree->getChild() << endl;
    cout << "tree child 2 " << tree->getChild()->getNext() << endl;
    cout << "tree list " << tl << endl;
    cout << "tree list 2 " << tl->getNext() << endl;
    cout << "tree list 3 " << tl->getNext()->getNext() << endl;
    if (debug) {
        cout << "Parsed tree" << endl;
        tree->display();
        cout << tree->str() << endl;
    }    
    // tree = solve(tree, debug);
    // if (debug) {
    //     cout << "Solved tree" << endl);
    //     printTree(tree);
    // }
    // cout << "Start expression : " << endl << expr << endl;
    // cout << "Final expression : " << endl << treeStr(tree) < endl;
    return 0;
}