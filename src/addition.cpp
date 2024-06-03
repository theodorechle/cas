#include "addition.hpp"
#include "number.hpp"
#include "multiplication.hpp"

bool Addition::solve(bool *delete_self) {
    Node *child1 = getChild();
    Node *child2 = child1->getNext();
    Token typeFirstChild = child1->getTokenType();
    Token typeSecondChild = child2->getTokenType();
    if (typeFirstChild == Token::Number && typeSecondChild == Token::Number) {
        Number *c1 = dynamic_cast<Number*>(child1);
        Number *c2 = dynamic_cast<Number*>(child2);
        c1->add(c2);
        c1->setNext(nullptr);
        delete c2;
        *delete_self = true;
        return true;
    }
    else if (typeFirstChild == Token::Variable && typeSecondChild == Token::Variable) {
        if (*child1 == *child2) {
            Node *newNode = new Multiplication;
            newNode->appendChild(new Number{"2"});
            newNode->appendChild(child2);
            child1->setNext(nullptr);
            replaceChild(child1, newNode);
            *delete_self = true;
            return true;
        }
    }
    return false;
}