using namespace std;

#include "parser.hpp"
#include "functions.hpp"
#include "constants.hpp"

using namespace constants;

bool isTypeOrEmpty(Types type, Types checkType) {
    return (type == Types::NUL || type == checkType);
}

bool isOperator(const string &value) {
    return OPERATORS.find(value) != OPERATORS.end() || REPLACE_OPERATORS.find(value) != REPLACE_OPERATORS.end();
}

void removeParenthesis(Node *t) {
    if (t->getType() == Types::CPA) t->replaceData(t->getChild());
    Node *child = t->getChild();
    while (child != nullptr) {
        removeParenthesis(child);
        child = child->getNext();
    }
}

void addTreeByValues(Node &t, const string &value, Types type) {
    if (type == Types::NUL) {
        throw NullError(value);
    }
    if (t.getType() == Types::NUL) {
        t.setValue(value);
        t.setType(type);
        return;
    }
    Node *tree = new Node{};
    tree->setValue(value);
    tree->setType(type);
    t.appendNext(tree);
}

bool tokenizeSpace(char character, string &value, bool &createNewTree) {
    if (isspace(character)) {
        if (value.empty()) createNewTree = false;
        return true;
    }
    return false;
}

bool tokenizeAlpha(char character, string &value, bool &createNewTree, int &index, Types &type, Node *exprList, bool implicitPriority) {
    if (isalpha(character)) {
        if (isTypeOrEmpty(type, Types::VAR)) {
                value += character;
                type = Types::VAR;
                createNewTree = false;
            }
            else {
                if (type == Types::NBR || type == Types::CPA) {
                    addTreeByValues(*exprList, value, type);
                    value.clear();
                    if (implicitPriority) value += IMPLICIT_MULTIPLICATION_SIGN;
                    else value += MULTIPLICATION_SIGN;
                    type = Types::OPT;
                }
                index--;
            }
            return true;
    }
    return false;
}

bool tokenizeOpeningParenthesis(char character, string &value, bool &createNewTree, int &index, Types &type, Node *exprList, bool implicitPriority) {
    if (character == OPENING_PARENTHESIS_SIGN[0]) {
        if (type == Types::NUL) type = Types::OPA;
        else {
            if (type == Types::NBR || type == Types::VAR) {
                addTreeByValues(*exprList, value, type);
                value.clear();
                if (implicitPriority) value += IMPLICIT_MULTIPLICATION_SIGN;
                else value += MULTIPLICATION_SIGN;
                type = Types::OPT;
            }
            index--;
        }
        return true;
    }
    return false;
}

bool tokenizeClosingParenthesis(char character, string &value, int &index, Types &type) {
    if (character == CLOSING_PARENTHESIS_SIGN[0]) {
        if (type == Types::NUL) type = Types::CPA;
        else index --;
        return true;
    }
    return false;
}

bool tokenizeNumber(char character, string &value, bool &createNewTree, int &index, Types &type) {
    if (isdigit(character) || character == '.') {
        if (isTypeOrEmpty(type, Types::NBR)) {
                value += character;
                type = Types::NBR;
                createNewTree = false;
        }
        else if (type == Types::VAR) {
            value += character;
            createNewTree = false;
        }
        else index --;
        return true;
    }
    return false;
}

bool tokenizeOperator(char character, string &value, bool &createNewTree, int &index, Types &type, string &testString) {
    if (type != Types::NUL && type != Types::OPT) index--;
    else {
        testString += value;
        testString += character;
        if (isOperator(testString)) {
            type = Types::OPT;
            value.clear();
            value += testString;
            createNewTree = false;
            testString.clear();
            return true;
        }
        createNewTree = false;
        testString.clear();
    }
    return false;
}

Node *tokenizer(string &expr, bool debug, bool implicitPriority) {
    Node *exprList = new Node{};
    string value{""};
    string testString{""};
    bool createNewTree;
    char character;
    int len = expr.length(), index = 0;
    Types type = Types::NUL;
    while (index < len) {
        createNewTree = true;
        character = expr[index];
        if (debug) cerr << "character : " << character << endl;
        if (tokenizeSpace(character, value, createNewTree)) {
            if (debug) cerr << "Found type of character '" << character << "' is space" << endl;
        }
        else if (tokenizeAlpha(character, value, createNewTree, index, type, exprList, implicitPriority)) {
            if (debug) cerr << "Found type of character '" << character << "' is alpha" << endl;
        }
        else if (tokenizeOpeningParenthesis(character, value, createNewTree, index, type, exprList, implicitPriority)) {
            if (debug) cerr << "Found type of character '" << character << "' is opening parenthesis" << endl;
        }
        else if (tokenizeClosingParenthesis(character, value, index, type)) {
            if (debug) cerr << "Found type of character '" << character << "' is closing parenthesis" << endl;
        }
        else if (tokenizeNumber(character, value, createNewTree, index, type)) {
            if (debug) cerr << "Found type of character '" << character << "' is number" << endl;
        }
        else {
            if (tokenizeOperator(character, value, createNewTree, index, type, testString)) {
                if (debug) cerr << "Found type of value '" << value << "' is operator" << endl;
            }
            else if (debug) cerr << "Type of value '" << value << "' not found" << endl;
            }
        if (debug) cerr << "value : " << value << " type : " << type << endl;
        if (createNewTree) {
            std::map<std::string, std::string>::const_iterator} index = REPLACE_OPERATORS.find(value);
            if (index != REPLACE_OPERATORS.cend()) value = index->second;
            addTreeByValues(*exprList, value, type);
            value.clear();
            type = Types::NUL;
        }
        index++;
    }
    if (!value.empty()) addTreeByValues(*exprList, value, type);
    return exprList;
}

Node *findRootOrParenthesis(Node *tree) {
    if (tree->getParent() == nullptr || tree->getParent()->getType() == Types::OPA) return tree;
    return findRootOrParenthesis(tree->getParent());
}

Node *parser(Node *exprList, bool debug, bool implicitPriority) {
    Node *tree = new Node{};
    Types treeType = Types::NUL;
    if (exprList == nullptr) return tree;
    while (exprList != nullptr) {
        treeType = exprList->getType();
        if (treeType == Types::NBR || treeType == Types::VAR) {
            tree->replaceData(exprList);
            tree = findRootOrParenthesis(tree);
        }
        else if (treeType == Types::OPT) {
            if ((exprList->getValue() == SUBSTRACTION_SIGN) && (tree == nullptr || treeType == Types::OPA)) {
                tree->replaceData(new Node{Types::NBR, "0"});
            }
            if (exprList->getValue() == IMPLICIT_MULTIPLICATION_SIGN) exprList->setValue(MULTIPLICATION_SIGN);
            if (tree->getType() != Types::OPT || getPriority(exprList->getValue()) <= getPriority(tree->getValue())) {
                exprList->addEmptyChild()->replaceData(tree);
                tree->replaceData(exprList);
                tree = tree->addEmptyChild();
            }
            else {
                Node *child;
                child = getLastChild(tree);
                while (child->getType() == Types::OPT && getPriority(exprList->getValue()) > getPriority(child->getValue()))
                    child = getLastChild(child);
                exprList->addEmptyChild()->replaceData(child);
                child->replaceData(exprList);
                tree = child->addEmptyChild();
            }
        }
        else if (treeType == Types::OPA) {
            if (tree->getType() != Types::NUL) tree = tree->addEmptyChild();
            tree->replaceData(exprList);
            tree = tree->addEmptyChild();
        }
        else if (treeType == Types::CPA) {
            tree = findRootOrParenthesis(tree);
            if (tree->getParent() != nullptr) {
                tree = tree->getParent();
                tree->setType(Types::CPA);
            }
            if (tree->getParent() != nullptr) tree = tree->getParent();
        }
        if (debug) {
            cerr << endl << "Root : " << endl;
            root(tree)->display(cerr);
            cerr << endl;
        }
        exprList = exprList->getNext();
    }
    removeParenthesis(tree);
    return root(tree);
}
