#include "addition.hpp"
#include "number.hpp"
#include "multiplication.hpp"
#include "substraction.hpp"


bool Addition::solve(bool *delete_self) {
    Node *child1 = getChild();
    if (child1 == nullptr or child1->getNext() == nullptr) {
        throw InvalidOperation("Can't addition with less than two values");
    }
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
    else {
        child1 = getFirstSubNumber(child1, getOperatorPriority(getTokenType()));
        child2 = getFirstSubNumber(child2, getOperatorPriority(getTokenType()));
        std::cerr << child1 << " " << child2 << std::endl;
        if (child1 == nullptr || child2 == nullptr) return false;
        bool substraction = false;
        if (dynamic_cast<Substraction *>(child1->getParent()) &&
                child1->getParent()->getChild() != child1) { // value is not the first child
            substraction = !substraction;
        }
        if (dynamic_cast<Substraction *>(child2->getParent()) &&
                child2->getParent()->getChild() != child2) { // value is not the first child
            substraction = !substraction;
        }
        Node *newNodes;
        if (substraction) newNodes = new Substraction();
        else newNodes = new Addition();
        newNodes->appendChild(child1->copyNodeWithChilds());
        newNodes->appendChild(child2->copyNodeWithChilds());

        child1->getParent()->replaceChild(child1, newNodes);

        Node *child2Parent = child2->getParent();
        if (child2Parent == this) {
            child2Parent->replaceData(child2Parent->getChild());
            return true;
        }

        child2Parent->removeSpecificChild(child2);

        child2Parent->getParent()->getParent()->replaceChild(child2Parent->getParent(), child2Parent->copyNodeWithChilds());

        *delete_self = true;
        return true;
    }
    return false;
}