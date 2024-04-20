#ifndef FLOAT_HPP
#define FLOAT_HPP

#include "node.hpp"

class Number: public Node {
    std::string integerPart;
    std::string fractionPart;
    bool negative;
    void ParseNumber();
    void setIntegerPart(const std::string &);
    void setFractionPart(const std::string &);
    std::string addPart(const std::string &thisNumber, const std::string &otherNumber, bool substract, bool *overflow);
public:
    Number(std::string number, Node *parent=nullptr): Node{constants::Types::NBR, number, parent} {
        ParseNumber();
    };
    ~Number() {};
    bool isNegative() const {return negative;};
    bool isGreaterThan(Number *) const;
    bool isLowerThan(Number *) const;
    bool isEqualTo(Number *) const;
    const std::string &getIntegerPart() const {return integerPart;}
    const std::string &getFractionPart() const {return fractionPart;}
    int getIntegerSize() const {return integerPart.size() - negative;}
    int getFractionSize() const {return fractionPart.size();}
    void Add(Number *n);
    void Substract(Number *n);
    void Multiply(Number *n);
    void Divide(Number *n);

};

std::ostream &operator<<(std::ostream &o, const Number &n);

#endif // FLOAT_HPP