#ifndef FLOAT_HPP
#define FLOAT_HPP

#include "node.hpp"

class Number: public Node {
    std::string integralPart;
    std::string decimalPart;
    bool negative;
    void ParseNumber();
    void setIntegralPart(const std::string &);
    void setDecimalPart(const std::string &);
    std::string addPart(const std::string &thisNumber, const std::string &otherNumber, bool substract, bool *overflow);
public:
    Number(std::string number, Node *parent=nullptr): Node{constants::Types::NBR, number, parent} {
        ParseNumber();
    };
    bool isNegative() const {return negative;};
    bool isGreaterThan(Number *) const;
    bool isLowerThan(Number *) const;
    bool isEqualTo(Number *) const;
    const std::string &getIntegralPart() const {return integralPart;}
    const std::string &getDecimalPart() const {return decimalPart;}
    void Add(Number *n);
    void Substract(Number *n);
    void Multiply(Number *n);
    void Divide(Number *n);
    std::string str() const;
};

std::ostream &operator<<(std::ostream &o, const Number &n);

#endif // FLOAT_HPP