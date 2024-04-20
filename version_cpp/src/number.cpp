#include <algorithm>

#include "number.hpp"

using namespace std;

void Number::ParseNumber() {
    negative = false;
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

bool Number::isGreaterThan(Number *n) const {
    if (isNegative() && !n->isNegative()) return false;
    if (!isNegative() && n->isNegative()) return true;
    // value is without '-'
    if (isNegative() && n->isNegative()) {
        Number thisCopy = Number(n->getValue());
        Number otherCopy = Number(getValue());
        return thisCopy.isLowerThan(&otherCopy);
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
    // value is without '-'
    if (isNegative() && n->isNegative()) {
        Number thisCopy = Number(n->getValue());
        Number otherCopy = Number(getValue());
        return thisCopy.isGreaterThan(&otherCopy);
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

void Number::Add(Number *n) {
    bool substract = isNegative() ^ n->isNegative();
    bool overflow = false;
    if (isLowerThan(n)) {
        setFractionPart(addPart(getFractionPart(), n->getFractionPart(), substract, &overflow));
        setIntegerPart(addPart(getIntegerPart(), n->getIntegerPart(), substract, &overflow));
    }
    else {
        setFractionPart(addPart(n->getFractionPart(), getFractionPart(), substract, &overflow));
        setIntegerPart(addPart(n->getIntegerPart(), getIntegerPart(), substract, &overflow));
    }
    if (overflow)
        setIntegerPart('1' + getIntegerPart());
    setValue((substract ? "-" : "") + getIntegerPart() + "." + getFractionPart());
    negative = substract;
}

string Number::addPart(const string &thisNumber, const string &otherNumber, bool substract, bool *overflow) {
    int zero = (int)'0';
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
        
        result += (char)(r + zero);
    }
    reverse(result.begin(), result.end());
    return result;


    /*Do it with int casts, char by char*/
}

void Number::Substract(Number *n) {

}

void Number::Multiply(Number *n) {

}

void Number::Divide(Number *n) {
    
}

std::ostream &operator<<(std::ostream &o, const Number &n) {
    o << n.getValue();
    return o;
}
