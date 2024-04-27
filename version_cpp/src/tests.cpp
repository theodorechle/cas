#include <string>
#include <iostream>

#include "node.hpp"
#include "parser.hpp"
#include "solver.hpp"
#include "tokenizer.hpp"
#include "number.hpp"

using namespace std;

void testStringEqual(const string &expr, const string &expected, Settings *settings) {
    cerr << "Test tokenizer + parser " << expr << " == \"" << expected << "\" : ";
    try {
        Node *tokens = Tokenizer(expr, settings).getResult();
        Node *result = Parser(tokens, settings).getFinalTree();
        if (result->str() == expected) cerr << "OK";
        else cerr << "KO";
        delete result;
        delete tokens;
    }
    catch (const exception& e) {
        cerr << "Failed with error : " << e.what();
    }
    cerr << endl;
}

void testStringDifferent(const string &expr, const string &notExpected, Settings *settings) {
    cerr << "Test tokenizer + parser " << expr << " != \"" << notExpected << "\" : ";
    try {
        Node *tokens = Tokenizer(expr, settings).getResult();
        Node *result = Parser(tokens, settings).getFinalTree();
        if (result->str() != notExpected) cerr << "OK";
        else cerr << "KO";
        delete result;
        delete tokens;
    }
    catch (const exception& e) {
        cerr << "Failed with error : " << e.what();
    }
    cerr << endl;
}

void testExprEqual(const string &expr, const string &expr2, Settings *settings) {
    cerr << "Test expr " << expr << " == " << expr2 << " : ";
    try {
        Node *tokens = Tokenizer(expr, settings).getResult();
        Node *result = Parser(tokens, settings).getFinalTree();
        Node *tokens2 = Tokenizer(expr2, settings).getResult();
        Node *result2 = Parser(tokens2, settings).getFinalTree();
        if (result->str() == result2->str()) cerr << "OK";
        else cerr << "KO";
        delete result;
        delete result2;
        delete tokens;
        delete tokens2;
    }
    catch (const exception& e) {
        cerr << "Failed with error : " << e.what();
    }
    cerr << endl;
}


void testExprDifferent(const string &expr, const string &expr2, Settings *settings) {
    cerr << "Test expr " << expr << " != " << expr2 << " : ";
    try {
        Node *tokens = Tokenizer(expr, settings).getResult();
        Node *result = Parser(tokens, settings).getFinalTree();
        Node *tokens2 = Tokenizer(expr2, settings).getResult();
        Node *result2 = Parser(tokens2, settings).getFinalTree();
        if (result->str() != result2->str()) cerr << "OK";
        else cerr << "KO";
        delete result;
        delete result2;
        delete tokens;
        delete tokens2;
    }
    catch (const exception& e) {
        cerr << "Failed with error : " << e.what();
    }
    cerr << endl;
}

void testEqualTokenizerNode(const string &expr, const Node *node, Settings *settings) {
    cerr << "Test tokenizer " << expr << " == node : ";
    try {
        Node *result = Tokenizer(expr, settings).getResult();
        Node *n = result;
        while (n != nullptr) {
            if (node == nullptr || !(*n == *node)) {
                cerr << "KO" << endl;
                delete result;
                return;
            }
            n = n->getNext();
            node = node->getNext();
        }
        if (node != nullptr) cerr << "KO";
        else cerr << "OK";
        delete result;
    }
    catch (const exception& e) {
        cerr << "Failed with error : " << e.what();
    }
    cerr << endl;
}


void testDifferentTokenizerNode(const string &expr, const Node *node, Settings *settings) {
    cerr << "Test tokenizer " << expr << " != node : ";
    try {
        Node *result = Tokenizer(expr, settings).getResult();
        Node *n = result;
        while (n != nullptr) {
            if (node == nullptr || *n == *node) {
                cerr << "KO" << endl;
                delete result;
                return;
            }
            n = n->getNext();
            node = node->getNext();
        }
        if (node != nullptr) cerr << "KO";
        else cerr << "OK";
        delete result;
    }
    catch (const exception& e) {
        cerr << "Failed with error : " << e.what();
    }
    cerr << endl;
}

void testEqualParserNode(Node *expr, const Node *node, Settings *settings) {
    cerr << "Test parser " << *expr << " == node " << *node << " : ";
    try {
        Node *result = Parser(expr, settings).getFinalTree();
        if (areSameNodes(result, node)) cerr << "OK";
        else cerr << "KO";
        delete result;
    }
    catch (const exception& e) {
        cerr << "Failed with error : " << e.what();
    }
    cerr << endl;
}


void testDifferentParserNode(Node *expr, const Node *node, Settings *settings) {
    cerr << "Test parser " << *expr << " != node " << *node << " : ";
    try {
        Node *result = Parser(expr, settings).getFinalTree();
        if (!areSameNodes(result, node)) cerr << "OK";
        else cerr << "KO";
        delete result;
    }
    catch (const exception& e) {
        cerr << "Failed with error : " << e.what();
    }
    cerr << endl;
}

void testEqualNode(const string &expr, const Node *node, Settings *settings) {
    cerr << "Test tokenizer + parser " << expr << " == node " << *node << " : ";
    try {
        Node *tokens = Tokenizer(expr, settings).getResult();
        Node *result = Parser(tokens, settings).getFinalTree();
        if (areSameNodes(result, node)) cerr << "OK";
        else cerr << "KO";
        delete result;
        delete tokens;
    }
    catch (const exception& e) {
        cerr << "Failed with error : " << e.what();
    }
    cerr << endl;
}


void testDifferentNode(const string &expr, const Node *node, Settings *settings) {
    cerr << "Test tokenizer + parser " << expr << " != node " << *node << " : ";
    try {
        Node *tokens = Tokenizer(expr, settings).getResult();
        Node *result = Parser(tokens, settings).getFinalTree();
        if (!areSameNodes(result, node)) cerr << "OK";
        else cerr << "KO";
        delete result;
        delete tokens;
    }
    catch (const exception& e) {
        cerr << "Failed with error : " << e.what();
    }
    cerr << endl;
}

string isBoolValid(bool a, bool b) {
    if (a == b) return "OK";
    return "Error, " + to_string(a) + " instead of " + to_string(b);
}

string areStringEquals(const string &a, const string &b) {
    if (a == b) return "OK";
    return "Error, " + a + " instead of " + b;
}

void isGreaterNumber(Number *a, Number *b, bool expected) {
    cerr << *a << " > " << *b << " : " << isBoolValid(a->isGreaterThan(b), expected) << endl;
}

void isLowerNumber(Number *a, Number *b, bool expected) {
    cerr << *a << " < " << *b << " : " << isBoolValid(a->isLowerThan(b), expected) << endl;
}

void isEqualNumber(Number *a, Number *b, bool expected) {
    cerr << *a << " == " << *b << " : " << isBoolValid(a->isEqualTo(b), expected) << endl;
}

int main() {
    Settings *settings = new Settings;
    settings->debug = false;
    testExprEqual("3x**2", "3x^2", settings);
    testExprDifferent("3x**2", "3x*2", settings);
    testStringEqual("3x**2", "3*x^2", settings);
    testStringDifferent("3x**2", "3x^2", settings);
    settings->implicitMultiplicationPriority = true;
    testStringEqual("3x**2*(-2)+3", "3*x^2*(0-2)+3", settings);
    settings->implicitMultiplicationPriority = false;
    Node *value = new Node{Token::Minus};
    Node *next1 = new Number{"3"};
    Node *next2 = new Node{Token::Name, "x"};
    value->appendNext(next1);
    next1->appendNext(next2);
    testEqualTokenizerNode("-3x", value, settings);
    delete value;
    Number *a = new Number{"53.9"};
    Number *a2 = new Number{"53.9"};
    Number *b = new Number{"-92.5"};
    Number *c = new Number{"-3.2"};
    Number *one = new Number{"1"};
    Number *zero = new Number{"0"};
    Number *zeroFloat = new Number{"0.0"};
    isGreaterNumber(a, b, true);
    isLowerNumber(b, a, true);
    isLowerNumber(a, b, false);
    isGreaterNumber(b, a, false);
    isGreaterNumber(a, a, false);
    isLowerNumber(a, a, false);
    isEqualNumber(a, a, true);
    isEqualNumber(b, b, true);
    isEqualNumber(a, b, false);
    isEqualNumber(b, a, false);
    cout << *a << " + " << *b << " : ";
    a->add(b);
    cout << areStringEquals(a->getValue(), "-38.6") << endl;
    cout << *b << " + " << *a2 << " : ";
    b->add(a2);
    cout << areStringEquals(b->getValue(), "-38.6") << endl;
    cout << *a2 << " + " << *a2 << " : ";
    a2->add(a2);
    cout << areStringEquals(a2->getValue(), "107.8") << endl;
    cout << *c << " + " << *c << " : ";
    c->add(c);
    cout << areStringEquals(c->getValue(), "-6.4") << endl;
    isEqualNumber(zero, zeroFloat, true);
    isGreaterNumber(zero, a, true);
    isLowerNumber(zero, c, false);
    cout << *zero << " + " << *one << " : ";
    zero->add(one);
    cout << areStringEquals(zero->getValue(), "1") << endl;
    cout << *zeroFloat << " + " << *one << " : ";
    zeroFloat->add(one);
    cout << areStringEquals(zeroFloat->getValue(), "1") << endl;
    delete a;
    delete a2;
    delete b;
    delete c;
    delete one;
    delete zero;
    delete zeroFloat;
    delete settings;
    return 0;
}
