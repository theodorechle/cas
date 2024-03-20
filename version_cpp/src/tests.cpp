#include <string>
#include <iostream>

#include "node.hpp"
#include "parser.hpp"
#include "solver.hpp"
#include "tokenizer.hpp"

using namespace std;

void test_string_equal(const string &expr, const string &expected, bool implicitPriority = false) {
    cerr << "Test " << expr << " == \"" << expected << "\" : ";
    try {
        const string result = parser(tokenizer(expr, false, implicitPriority), false, implicitPriority)->str();
        if (result == expected) cerr << "OK";
        else cerr << "KO";
    }
    catch (...) {
        cout << "Failed with error";
    }
    cerr << endl;
}

void test_string_different(const string &expr, const string &notExpected, bool implicitPriority = false) {
    cerr << "Test " << expr << " != \"" << notExpected << "\" : ";
    try {
        const string result = parser(tokenizer(expr, false, implicitPriority), false, implicitPriority)->str();
        if (result != notExpected) cerr << "OK";
        else cerr << "KO";
    }
    catch (...) {
        cout << "Failed with error";
    }
    cerr << endl;
}

void test_equal(const string &expr, const string &expr2, bool implicitPriority = false) {
    cerr << "Test " << expr << " == " << expr2 << " : ";
    try {
        const string result = parser(tokenizer(expr, false, implicitPriority), false, implicitPriority)->str();
        const string result2 = parser(tokenizer(expr2, false, implicitPriority), false, implicitPriority)->str();
        if (result == result2) cerr << "OK";
        else cerr << "KO";
    }
    catch (...) {
        cout << "Failed with error";
    }
    cerr << endl;
}


void test_different(const string &expr, const string &expr2, bool implicitPriority = false) {
    cerr << "Test " << expr << " != " << expr2 << " : ";
    try {
        const string result = parser(tokenizer(expr, false, implicitPriority), false, implicitPriority)->str();
        const string result2 = parser(tokenizer(expr2, false, implicitPriority), false, implicitPriority)->str();
        if (result != result2) cerr << "OK";
        else cerr << "KO";
    }
    catch (...) {
        cout << "Failed with error";
    }
    cerr << endl;
}



int main() {
    test_equal("3x**2", "3x^2");
    test_different("3x**2", "3x*2");
    test_string_equal("3x**2", "3*x^2");
    test_string_different("3x**2", "3x^2");
    return 0;
}
