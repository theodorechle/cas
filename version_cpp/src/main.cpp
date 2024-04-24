#include "tokenizer.hpp"
#include "parser.hpp"
#include "solver.hpp"

using namespace std;

int main() {
    bool debug, implicitMultiplicationPriority;
    string expr;
    Node *tl;
    cout << "Expression : ";
    getline(cin, expr);
    cout << "1 for debug, 0 else : ";
    cin >> debug;
    cout << "1 for implicit multiplication priority, 0 else : ";
    cin >> implicitMultiplicationPriority;
    tl = tokenizer(expr);

    if (debug) {
        Node *tmp = tl;
        cerr << "Tokens" << endl;
        while (tmp != nullptr) {
            tmp->display(cerr);
            tmp = tmp->getNext();
        }
    }
    Node *tree;
    tree = parser(tl, debug, implicitMultiplicationPriority);
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
