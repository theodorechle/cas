#include "tokenizer.hpp"
#include "parser.hpp"
#include "solver.hpp"

using namespace std;

int main() {
    Settings *settings = new Settings;
    bool debug, implicitMultiplicationPriority;
    string *expr = new string;
    Node *tokens;
    cout << "Expression : ";
    getline(cin, *expr);
    cout << "1 for debug, 0 else : ";
    cin >> debug;
    cout << "1 for implicit multiplication priority, 0 else : ";
    cin >> implicitMultiplicationPriority;
    Tokenizer *tokenizer = new Tokenizer{*expr, settings};
    tokens = tokenizer->getResult();
    delete tokenizer;
    if (debug) {
        Node *tmp = tokens;
        cerr << "Tokens" << endl;
        while (tmp != nullptr) {
            tmp->display(cerr);
            tmp = tmp->getNext();
        }
    }
    Node *tree;
    Parser *parser = new Parser{tokens, settings};
    tree = parser->getFinalTree();
    delete parser;
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
    cout << "Start expression : " << endl << *expr << endl;
    cout << "Final expression : " << endl << *tree << endl;
    delete expr;
    delete tree;
    delete settings;
    return 0;
}
