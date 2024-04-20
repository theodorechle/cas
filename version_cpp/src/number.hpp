#ifndef FLOAT_HPP
#define FLOAT_HPP

#include "node.hpp"

class Number: public Node {
    std::string integerPart;
    std::string fractionPart;
    bool negative;
    void parseNumber();
    void setIntegerPart(const std::string &);
    void setFractionPart(const std::string &);
    std::string addPart(const std::string &thisNumber, const std::string &otherNumber, bool substract, bool *overflow);
public:
    Number(std::string number, Node *parent=nullptr): Node{constants::Types::NBR, number, parent} {
        parseNumber();
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
    void add(Number *n);
    void substract(Number *n);
    void multiply(Number *n);
    void divide(Number *n);

};

std::ostream &operator<<(std::ostream &o, const Number &n);

#endif // FLOAT_HPP