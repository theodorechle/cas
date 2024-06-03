#ifndef OPERATOR_HPP
#define OPERATOR_HPP

#include "node.hpp"

class Operator: public Node {
public:
    Operator(Token token, Node *parent = nullptr): Node{token, "", parent} {}
    /**
     * Return true if the tree was updated else false
     * delete_self is because the node can't delete itself, the calling function will do it instead
    */
    virtual bool solve(bool *delete_self) = 0;
};

/**
 * Check if the two tokens are equal regardless of the order (node1 can be equal to expected1 or 2 and same for node2)
 * They can't be only one of the expecteds
*/
bool areSameTokens(Token node1, Token node2, Token expected1, Token expected2);

bool isOneTokenTheSame(Token token1, Token token2, Token expected);

#endif // OPERATOR_HPP