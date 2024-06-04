#include "addition.hpp"
#include "number.hpp"
#include "multiplication.hpp"
#include "substraction.hpp"

Node *getFirstSubNumber(Node *self) {
    std::cerr << "self : " << *self << std::endl;
    if (!isOperator(self->getTokenType())) return nullptr;
    std::cerr << "here" << std::endl;
    Node *child = self->getChild();
    while (child != nullptr) {
        std::cerr << "child : " << *child << std::endl;
        if (!isOperator(child->getTokenType())) {
            if (dynamic_cast<Number *>(child) != nullptr) return child;
            continue;
        }
        std::cerr << "still here" << std::endl;
        if (getOperatorPriority(child->getTokenType()) > getOperatorPriority(self->getTokenType())) continue;
        std::cerr << "and still here" << std::endl;
        Node *foundNumber = getFirstSubNumber(child);
        if (foundNumber != nullptr) return foundNumber;
        std::cerr << "it seems no number has been found yet" << std::endl;
        child = child->getNext();
    }
    return nullptr;
}

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
    else {
        Node *value;
        if (isOperator(child1->getTokenType()) && dynamic_cast<Number *>(child2) != nullptr) {
            value = getFirstSubNumber(child1);
        }
        else if (isOperator(child2->getTokenType()) && dynamic_cast<Number *>(child1) != nullptr) {
            value = getFirstSubNumber(child2);
        }
        else return false;
        std::cerr << "value : ";
        std::cerr << value << std::endl;
        if (value != nullptr) {
            std::cerr << *value << std::endl;
        }
        if (value != nullptr) {
            Node *newNodes;
            if (dynamic_cast<Substraction *>(value->getParent()) && value->getParent()->getChild() != value) newNodes = new Substraction();
            else newNodes = new Addition();
            newNodes->appendChild(value->copyNodeWithChilds());
            newNodes->appendChild(child2->copyNodeWithChilds());
            value->getParent()->replaceChild(value, newNodes);
            *delete_self = true;
            return true;
        }

        
    }
    return false;
}