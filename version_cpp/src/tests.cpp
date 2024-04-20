#include <string>
#include <iostream>

#include "node.hpp"
#include "parser.hpp"
#include "solver.hpp"
#include "tokenizer.hpp"
#include "number.hpp"

using namespace std;
using namespace constants;

void testStringEqual(const string &expr, const string &expected, bool implicitPriority = false) {
    cerr << "Test tokenizer + parser " << expr << " == \"" << expected << "\" : ";
    try {
        const string result = parser(tokenizer(expr, false, implicitPriority), false, implicitPriority)->str();
        if (result == expected) cerr << "OK";
        else cerr << "KO";
    }
    catch (const exception& e) {
        cout << "Failed with error : " << e.what();
    }
    cerr << endl;
}

void testStringDifferent(const string &expr, const string &notExpected, bool implicitPriority = false) {
    cerr << "Test tokenizer + parser " << expr << " != \"" << notExpected << "\" : ";
    try {
        const string result = parser(tokenizer(expr, false, implicitPriority), false, implicitPriority)->str();
        if (result != notExpected) cerr << "OK";
        else cerr << "KO";
    }
    catch (const exception& e) {
        cout << "Failed with error : " << e.what();
    }
    cerr << endl;
}

void testExprEqual(const string &expr, const string &expr2, bool implicitPriority = false) {
    cerr << "Test expr " << expr << " == " << expr2 << " : ";
    try {
        const string result = parser(tokenizer(expr, false, implicitPriority), false, implicitPriority)->str();
        const string result2 = parser(tokenizer(expr2, false, implicitPriority), false, implicitPriority)->str();
        if (result == result2) cerr << "OK";
        else cerr << "KO";
    }
    catch (const exception& e) {
        cout << "Failed with error : " << e.what();
    }
    cerr << endl;
}


void testExprDifferent(const string &expr, const string &expr2, bool implicitPriority = false) {
    cerr << "Test expr " << expr << " != " << expr2 << " : ";
    try {
        const string result = parser(tokenizer(expr, false, implicitPriority), false, implicitPriority)->str();
        const string result2 = parser(tokenizer(expr2, false, implicitPriority), false, implicitPriority)->str();
        if (result != result2) cerr << "OK";
        else cerr << "KO";
    }
    catch (const exception& e) {
        cout << "Failed with error : " << e.what();
    }
    cerr << endl;
}

void testEqualTokenizerNode(const string &expr, const Node *node, bool implicitPriority = false) {
    cerr << "Test tokenizer " << expr << " == node : ";
    try {
        const Node *result = tokenizer(expr, false, implicitPriority);
        while (result != nullptr) {
            if (node == nullptr || !(*result == *node)) {
                cerr << "KO" << endl;
                return;
            }
            result = result->getNext();
            node = node->getNext();
        }
        if (node != nullptr) cerr << "KO";
        else cerr << "OK";
    }
    catch (const exception& e) {
        cout << "Failed with error : " << e.what();
    }
    cerr << endl;
}


void testDifferentTokenizerNode(const string &expr, const Node *node, bool implicitPriority = false) {
    cerr << "Test tokenizer " << expr << " != node : ";
    try {
        const Node *result = tokenizer(expr, false, implicitPriority);
        while (result != nullptr) {
            if (node == nullptr || *result == *node) {
                cerr << "KO" << endl;
                return;
            }
            result = result->getNext();
            node = node->getNext();
        }
        if (node != nullptr) cerr << "KO";
        else cerr << "OK";
    }
    catch (const exception& e) {
        cout << "Failed with error : " << e.what();
    }
    cerr << endl;
}

void testEqualParserNode(Node *expr, const Node *node, bool implicitPriority = false) {
    cerr << "Test parser " << *expr << " == node " << *node << " : ";
    try {
        const Node *result = parser(expr, false, implicitPriority);
        if (areSameNodes(result, node)) cerr << "OK";
        else cerr << "KO";
    }
    catch (const exception& e) {
        cout << "Failed with error : " << e.what();
    }
    cerr << endl;
}


void testDifferentParserNode(Node *expr, const Node *node, bool implicitPriority = false) {
    cerr << "Test parser " << *expr << " != node " << *node << " : ";
    try {
        const Node *result = parser(expr, false, implicitPriority);
        if (!areSameNodes(result, node)) cerr << "OK";
        else cerr << "KO";
    }
    catch (const exception& e) {
        cout << "Failed with error : " << e.what();
    }
    cerr << endl;
}

void testEqualNode(const string &expr, const Node *node, bool implicitPriority = false) {
    cerr << "Test tokenizer + parser " << expr << " == node " << *node << " : ";
    try {
        const Node *result = parser(tokenizer(expr, false, implicitPriority), false, implicitPriority);
        if (areSameNodes(result, node)) cerr << "OK";
        else cerr << "KO";
    }
    catch (const exception& e) {
        cout << "Failed with error : " << e.what();
    }
    cerr << endl;
}


void testDifferentNode(const string &expr, const Node *node, bool implicitPriority = false) {
    cerr << "Test tokenizer + parser " << expr << " != node " << *node << " : ";
    try {
        const Node *result = parser(tokenizer(expr, false, implicitPriority), false, implicitPriority);
        if (!areSameNodes(result, node)) cerr << "OK";
        else cerr << "KO";
    }
    catch (const exception& e) {
        cout << "Failed with error : " << e.what();
    }
    cerr << endl;
}

const string isBoolValid(bool a, bool b) {
    if (a == b) return "OK";
    return "Error, " + to_string(a) + " instead of " + to_string(b);
}

const string isStringValid(const string &a, const string &b) {
    if (a == b) return "OK";
    return "Error, " + a + " instead of " + b;
}

int main() {
    testExprEqual("3x**2", "3x^2");
    testExprDifferent("3x**2", "3x*2");
    testStringEqual("3x**2", "3*x^2");
    testStringDifferent("3x**2", "3x^2");
    Node *value = new Node{Types::OPT, "-"};
    Node *child1 = new Node{Types::NBR, "3"};
    Node *child2 = new Node{Types::OPT, "*"};
    Node *child3 = new Node{Types::VAR, "x"};
    value->appendNext(child1);
    child1->appendNext(child2);
    child2->appendNext(child3);
    testEqualTokenizerNode("-3x", value);
    delete value;
    Number a{"53.9"};
    Number a2{"53.9"};
    Number b{"-92.5"};
    cout << a << " > " << b << " : " << isBoolValid(a.isGreaterThan(&b), true) << endl;
    cout << b << " < " << a << " : " << isBoolValid(b.isLowerThan(&a), true) << endl;
    cout << a << " < " << b << " : " << isBoolValid(a.isLowerThan(&b), false) << endl;
    cout << b << " > " << a << " : " << isBoolValid(b.isGreaterThan(&a), false) << endl;
    cout << a << " > " << a << " : " << isBoolValid(a.isGreaterThan(&a), false) << endl;
    cout << a << " < " << a << " : " << isBoolValid(a.isLowerThan(&a), false) << endl;
    cout << a << " = " << a << " : " << isBoolValid(a.isEqualTo(&a), true) << endl;
    cout << b << " = " << b << " : " << isBoolValid(b.isEqualTo(&b), true) << endl;
    cout << a << " = " << b << " : " << isBoolValid(a.isEqualTo(&b), false) << endl;
    cout << b << " = " << a << " : " << isBoolValid(b.isEqualTo(&a), false) << endl;
    cout << a << " + " << b << " : ";
    a.Add(&b);
    cout << isStringValid(a.getValue(), "-38.6") << endl;
    cout << b << " + " << a2 << " : ";
    b.Add(&a2);
    cout << isStringValid(b.getValue(), "-38.6") << endl;
    return 0;
}
