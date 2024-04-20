#include <algorithm>

#include "number.hpp"

using namespace std;

void Number::ParseNumber() {
    negative = false;
    string v = getValue();
    string integral;
    string decimal;
    string *part = &integral;
    if (v[0] == '-') {
        negative = true;
        v.erase(0, 1);
        setValue(v);
    }
    for (char c : v) {
        if (c == '.') {
            part = &decimal;
            continue;
        }
        *part += c;
    }
    setIntegralPart(integral);
    setDecimalPart(decimal);
}

void Number::setIntegralPart(const std::string &value) {
    bool beginning = true;
    integralPart.clear();
    for (char c : value) {
        if (beginning) {
            if (c != '0') {
                beginning = false;
                integralPart += c;
            }
        }
        else integralPart += c;
    }
}

void Number::setDecimalPart(const std::string &value) {
    bool ending = true;
    decimalPart.clear();
    for (string::const_reverse_iterator c = value.crbegin(); c != value.crend(); c++) {
        if (ending) {
            if (*c != '0') {
                ending = false;
                decimalPart += *c;
            }
        }
        else decimalPart += *c;
    }
    reverse(decimalPart.begin(), decimalPart.end());
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
    if (getIntegralPart().size() > n->getIntegralPart().size()) return true;
    if (getIntegralPart().size() < n->getIntegralPart().size()) return false;
    string::const_iterator thisChar = getIntegralPart().cbegin();
    string::const_iterator otherChar = n->getIntegralPart().cbegin();
    while (thisChar != getIntegralPart().cend()) {
        if (*thisChar > *otherChar) return true;
        if (*thisChar < *otherChar) return false;
        thisChar++;
        otherChar++;
    }
    thisChar = getDecimalPart().cbegin();
    otherChar = n->getDecimalPart().cbegin();
    while (thisChar != getDecimalPart().cend() && otherChar != n->getDecimalPart().cend()) {
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
    if (getIntegralPart().size() < n->getIntegralPart().size()) return true;
    if (getIntegralPart().size() > n->getIntegralPart().size()) return false;
    string::const_iterator thisChar = getIntegralPart().cbegin();
    string::const_iterator otherChar = n->getIntegralPart().cbegin();
    while (thisChar != getIntegralPart().cend()) {
        if (*thisChar < *otherChar) return true;
        if (*thisChar > *otherChar) return false;
        thisChar++;
        otherChar++;
    }
    thisChar = getDecimalPart().cbegin();
    otherChar = n->getDecimalPart().cbegin();
    while (thisChar != getDecimalPart().cend() && otherChar != n->getDecimalPart().cend()) {
        if (*thisChar < *otherChar) return true;
        if (*thisChar > *otherChar) return false;
        thisChar++;
        otherChar++;
    }
    return false;
}

bool Number::isEqualTo(Number *n) const {
    if (isNegative() != n->isNegative()) return false;
    if (getIntegralPart().size() != n->getIntegralPart().size()) return false;
    string::const_iterator thisChar = getIntegralPart().cbegin();
    string::const_iterator otherChar = n->getIntegralPart().cbegin();
    while (thisChar != getIntegralPart().cend()) {
        if (*thisChar != *otherChar) return false;
        thisChar++;
        otherChar++;
    }
    thisChar = getDecimalPart().cbegin();
    otherChar = n->getDecimalPart().cbegin();
    while (thisChar != getDecimalPart().cend() && otherChar != n->getDecimalPart().cend()) {
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
        setDecimalPart(addPart(getDecimalPart(), n->getDecimalPart(), substract, &overflow));
        setIntegralPart(addPart(getIntegralPart(), n->getIntegralPart(), substract, &overflow));
    }
    else {
        setDecimalPart(addPart(n->getDecimalPart(), getDecimalPart(), substract, &overflow));
        setIntegralPart(addPart(n->getIntegralPart(), getIntegralPart(), substract, &overflow));
    }
    if (overflow)
        setIntegralPart('1' + getIntegralPart());
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

string Number::str() const {
    string str = (isNegative() ? "-" : "") + getIntegralPart();
    const string &decimal = getDecimalPart();
    if (decimal.size()) {
        str += '.' + decimal;
    }
    return str;
}

std::ostream &operator<<(std::ostream &o, const Number &n) {
    o << n.str();
    return o;
}
