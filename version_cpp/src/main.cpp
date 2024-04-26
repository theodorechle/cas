#include "tokenizer.hpp"
#include "parser.hpp"
#include "solver.hpp"

using namespace std;

int main() {
    Settings *settings = new Settings;
    bool debug, implicitMultiplicationPriority;
    string expr;
    Node *tokens;
    cout << "Expression : ";
    getline(cin, expr);
    cout << "1 for debug, 0 else : ";
    cin >> debug;
    cout << "1 for implicit multiplication priority, 0 else : ";
    cin >> implicitMultiplicationPriority;
    tokens = tokenizer(expr);

    if (debug) {
        Node *tmp = tokens;
        cerr << "Tokens" << endl;
        while (tmp != nullptr) {
            tmp->display(cerr);
            tmp = tmp->getNext();
        }
    }
    Parser *parser = new Parser{tokens, settings, debug, implicitMultiplicationPriority};
    Node *tree;
    tree = parser->parse();
    delete tokens;
    if (debug) {
        cerr << "Parsed tree" << endl;
        tree->display(cerr);
        cerr << tree->str() << endl;
    }
    // tree = solve(tree, debug);
    if (debug) {
        cerr << "Solved tree" << endl;
        tree->display(cerr);
    }
    cout << "Start expression : " << endl << expr << endl;
    cout << "Final expression : " << endl << *tree << endl;
    delete tree;
    return 0;
}
