#include <algorithm>

#include "number.hpp"

using namespace std;

void Number::setValue(const std::string &s) {
    Node::setValue(s);
    parseNumber();
}

void Number::parseNumber() {
    string v = getValue();
    string integer;
    string fraction;
    string *part = &integer;
    if (v[0] == '-') {
        negative = true;
        v.erase(0, 1);
    }
    for (char c : v) {
        if (c == '.') {
            part = &fraction;
            continue;
        }
        *part += c;
    }
    setIntegerPart(integer);
    setFractionPart(fraction);
    updatevalue();
}

void Number::setIntegerPart(const std::string &value) {
    bool beginning = true;
    integerPart.clear();
    for (char c : value) {
        if (beginning) {
            if (c != '0') {
                beginning = false;
                integerPart += c;
            }
        }
        else integerPart += c;
    }
    if (integerPart.empty()) integerPart = "0";
}

void Number::setFractionPart(const std::string &value) {
    bool ending = true;
    fractionPart.clear();
    for (string::const_reverse_iterator c = value.crbegin(); c != value.crend(); c++) {
        if (ending) {
            if (*c != '0') {
                ending = false;
                fractionPart += *c;
            }
        }
        else fractionPart += *c;
    }
    reverse(fractionPart.begin(), fractionPart.end());
}

void Number::updatevalue() {
    Node::setValue((negative ? "-" : "") + getIntegerPart() + (!getFractionPart().empty() ? "." + getFractionPart() : ""));
}

bool Number::isGreaterThan(Number *n) const {
    if (isNegative() && !n->isNegative()) return false;
    if (!isNegative() && n->isNegative()) return true;
    if (isNegative() && n->isNegative()) {
        Number *thisCopy = n->getAbsolute();
        Number *otherCopy = getAbsolute();
        bool result = thisCopy->isLowerThan(otherCopy);
        delete thisCopy;
        delete otherCopy;
        return result;
    }
    if (getIntegerSize() > n->getIntegerSize()) return true;
    if (getIntegerSize() < n->getIntegerSize()) return false;
    string::const_iterator thisChar = getIntegerPart().cbegin();
    string::const_iterator otherChar = n->getIntegerPart().cbegin();
    while (thisChar != getIntegerPart().cend()) {
        if (*thisChar > *otherChar) return true;
        if (*thisChar < *otherChar) return false;
        thisChar++;
        otherChar++;
    }
    thisChar = getFractionPart().cbegin();
    otherChar = n->getFractionPart().cbegin();
    while (thisChar != getFractionPart().cend() && otherChar != n->getFractionPart().cend()) {
        if (*thisChar > *otherChar) return true;
        if (*thisChar < *otherChar) return false;
        thisChar++;
        otherChar++;
    }
    return false;
}

bool Number::isLowerThan(Number *n) const {
    if (!isNegative() && n->isNegative()) return false;
    if (isNegative() && n->isNegative()) {
        Number *thisCopy = n->getAbsolute();
        Number *otherCopy = getAbsolute();
        bool result = thisCopy->isGreaterThan(otherCopy);
        delete thisCopy;
        delete otherCopy;
        return result;
    }
    if (isNegative() && !n->isNegative()) return true;
    if (getIntegerSize() < n->getIntegerSize()) return true;
    if (getIntegerSize() > n->getIntegerSize()) return false;
    string::const_iterator thisChar = getIntegerPart().cbegin();
    string::const_iterator otherChar = n->getIntegerPart().cbegin();
    while (thisChar != getIntegerPart().cend()) {
        if (*thisChar < *otherChar) return true;
        if (*thisChar > *otherChar) return false;
        thisChar++;
        otherChar++;
    }
    thisChar = getFractionPart().cbegin();
    otherChar = n->getFractionPart().cbegin();
    while (thisChar != getFractionPart().cend() && otherChar != n->getFractionPart().cend()) {
        if (*thisChar < *otherChar) return true;
        if (*thisChar > *otherChar) return false;
        thisChar++;
        otherChar++;
    }
    return false;
}

bool Number::isEqualTo(Number *n) const {
    if (isNegative() != n->isNegative()) return false;
    if (getIntegerSize() != n->getIntegerSize()) return false;
    string::const_iterator thisChar = getIntegerPart().cbegin();
    string::const_iterator otherChar = n->getIntegerPart().cbegin();
    while (thisChar != getIntegerPart().cend()) {
        if (*thisChar != *otherChar) return false;
        thisChar++;
        otherChar++;
    }
    thisChar = getFractionPart().cbegin();
    otherChar = n->getFractionPart().cbegin();
    while (thisChar != getFractionPart().cend() && otherChar != n->getFractionPart().cend()) {
        if (*thisChar != *otherChar) return false;
        thisChar++;
        otherChar++;
    }
    return true;
}

Number *Number::getAbsolute() const {
    if (isNegative()) return new Number(getValue().substr(1));
    return new Number(getValue());
}

void Number::add(Number *n) {
    bool substract = isNegative() ^ n->isNegative();
    bool negativeSign = false;
    Number *absolute = getAbsolute();
    if (isNegative() && !n->isNegative() && absolute->isGreaterThan(n)) negativeSign = true;
    delete absolute;
    absolute = n->getAbsolute();
    if (!isNegative() && n->isNegative() && isLowerThan(absolute)) negativeSign = true;
    delete absolute;
    if (isNegative() && n->isNegative()) negativeSign = true;
    bool overflow = false;
    string integer;
    if (isLowerThan(n)) {
        setFractionPart(addPart(getFractionPart(), n->getFractionPart(), substract, &overflow));
        integer = addPart(getIntegerPart(), n->getIntegerPart(), substract, &overflow);
    }
    else {
        setFractionPart(addPart(n->getFractionPart(), getFractionPart(), substract, &overflow));
        integer = addPart(n->getIntegerPart(), getIntegerPart(), substract, &overflow);
    }
    if (overflow)
        integer = '1' + integer;
    setIntegerPart(integer);
    negative = negativeSign;
    updatevalue();
}

string Number::addPart(const string &thisNumber, const string &otherNumber, bool substract, bool *overflow) {
    int zero = '0';
    string result;
    int r;
    int thisSize = thisNumber.size();
    int otherSize = otherNumber.size();
    // add digits at same index for both
    int size = max(thisSize, otherSize);
    for (int index=1; index<=size; index++) {
        int thisDigit = thisSize >= index ? thisNumber[thisSize-index] - zero : 0;
        int otherDigit = otherSize >= index ? otherNumber[otherSize-index] - zero : 0;
        r = substract ? thisDigit - otherDigit - *overflow : thisDigit + otherDigit + *overflow;
        *overflow = false;
        if (r > 9) {
            *overflow = true;
            r -= 10;
        }
        else if (r < 0)
        {
            *overflow = true;
            r += 10;
        }
        
        result += (r + zero);
    }
    reverse(result.begin(), result.end());
    return result;


    /*Do it with int casts, char by char*/
}

void Number::substract(Number *n) {

}

void Number::multiply(Number *n) {

}

void Number::divide(Number *n) {
    
}

std::ostream &operator<<(std::ostream &o, const Number &n) {
    o << n.getValue();
    return o;
}
