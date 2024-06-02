#ifndef NUMBER_HPP
#define NUMBER_HPP

#include "node.hpp"

class Number: public Node {
    std::string integerPart;
    std::string fractionPart;
    bool negative = false;
    void parseNumber();
    void setIntegerPart(const std::string &);
    void setFractionPart(const std::string &);
    /**
     * Update the base node value with new negative, integerPart and fractionPart values.
     * The value attribute is used to have a representing string of the Number.
     * The updateValue method must be called after each modification of the inner attributes.
    */
    void updatevalue();
    std::string addPart(const std::string &thisNumber, const std::string &otherNumber, bool substract, bool *overflow);
public:
    Number(std::string number, Node *parent=nullptr): Node{Token::Number, number, parent} {
        parseNumber();
    };
    ~Number() {};
    bool isNegative() const {return negative;};
    bool isGreaterThan(Number *) const;
    bool isLowerThan(Number *) const;
    bool isEqualTo(Number *) const;
    const std::string &getIntegerPart() const {return integerPart;}
    const std::string &getFractionPart() const {return fractionPart;}
    Number *getAbsolute() const;
    int getIntegerSize() const {return integerPart.size();}
    int getFractionSize() const {return fractionPart.size();}
    void add(Number *n);
    void substract(Number *n);
    void multiply(Number *n);
    void divide(Number *n);
    Node *createNewNode() const override {return new Number("");}
};

std::ostream &operator<<(std::ostream &o, const Number &n);

#endif // NUMBER_HPP