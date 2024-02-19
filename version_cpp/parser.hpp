#include <string>

#include "constants.hpp"
#include "tree.hpp"

using namespace std;

using namespace constants;

class NullError : public exception {
    string msg;
public:
    NullError(string msg): msg{msg} {};
    const char *what() const noexcept override {string m = "Unknown value '"+msg+"'"; return m.c_str();};
};

bool isTypeOrEmpty(Types type, Types checkType);

bool isperator(string *value);


void addTreeByValues(Node &t, string *value, Types type);

Node *parser(string &expr, bool debug, bool implicitPriority);

Node *findRootOrParenthesis(Node *tree);

/**
  *Transform a chain of trees (no childs) like the one the parser function returns into a tree containing the entire expression
*/
Node *parsedToTree(Node *exprList, bool debug, bool implicitPriority);
