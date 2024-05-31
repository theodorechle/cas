#include "addition.hpp"
#include "number.hpp"

bool Addition::solve(bool *delete_self) {
    Node *child1 = getChild();
    Node *child2 = child1->getNext();
    Token typeFirstChild = child1->getTokenType();
    Token typeSecondChild = child2->getTokenType();
    if (isOneTokenTheSame(typeFirstChild, typeSecondChild, Token::Function)) return false;
    if (typeFirstChild == Token::Number && typeSecondChild == Token::Number) {
        Number *c1 = dynamic_cast<Number*>(child1);
        Number *c2 = dynamic_cast<Number*>(child2);
        std::cout << c1 << std::endl;
        std::cout << c2 << std::endl;
        c1->add(c2);
        std::cerr << "addition done : " << getChild() << std::endl;
        *delete_self = true;
        return true;
    }
    return false;
}