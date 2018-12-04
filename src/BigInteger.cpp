#include <algorithm>
#include <cctype>
#include <string>
#include <sstream>
#include <vector>

#include "BigInteger.hpp"


/*** CONSTRUCTORS ***/

bnum::BigInteger::BigInteger() {}

bnum::BigInteger::BigInteger(const std::string &bigIntInput)
{
    bool startFromOne;
    if(!isalpha(bigIntInput[0]) && (bigIntInput[0]=='+' || bigIntInput[0]=='-')) {
        this->sign=bigIntInput[0];
        startFromOne=true;
    } else { 
        this->sign='+';
        startFromOne=false;
    }

    for(size_t i = (startFromOne ? 1 : 0); i<bigIntInput.size(); ++i) {
        this->value.push_back(bigIntInput[i]-'0');
    }
}

bnum::BigInteger::BigInteger(const long long int &bigIntInput)
{
    long long int aux = bigIntInput;
    if(bigIntInput >= 0) { this->sign='+'; }
    else { 
        this->sign='-';
        aux = -aux;
    }

    while(aux!=0) {
        this->value.push_back(static_cast<digit_t>(aux%10));
        aux/=10;
    }

    std::reverse(this->value.begin(), this->value.end());
}

bnum::BigInteger::BigInteger(const BigInteger& bint)
{
    this->sign=bint.sign;
    this->value=bint.value;
}

/*** PRIVATE HELPERS ***/

void bnum::BigInteger::helpMult_(std::vector<digit_t> &bigger, const std::vector<digit_t> &smaller)
{
    size_t i, j;
    for(i = smaller.size()-1; i>=0; --i) {
        for(j = bigger.size()-1; j>0; --j) {
            bigger[j] *= smaller[i];
            if(bigger[j] > 9) {
                bigger[j+1] += bigger[j] % 10;
                bigger[j] /= 10;
            }
        }
        if(bigger[0] > 10) {
            digit_t tmp = bigger[0];
            bigger[0] %= 10;
            bigger.insert(bigger.begin(), tmp/10);
        }
    }
}


/*** OPERATOR OVERLOADS ***/

/***********MEMBER*********/
bnum::BigInteger& bnum::BigInteger::operator=(bnum::BigInteger rhs)
{
    swap(*this, rhs);
    return *this;
}

bnum::BigInteger& bnum::BigInteger::operator++()
{
    this->value[this->value.back()]++;
    if(this->value[this->value.back()] < 10) { return *this; }
    else {
        bool done = false;
        for(auto it = this->value.rbegin(); it != this->value.rend()-2 && !done; ++it) {
            if(*it > 10 ) {
                it+1 += (*it) % 10;
                *it /= 10;
            } else { done = true; }
        }
    }
    if(*(this->value.begin()) > 9)
    {
        digit_t tmp=value[0];
        this->value.insert(this->value.begin(), tmp/10);
        *(this->value.begin()+1)=tmp%10;
    }
    return *this;
}

bnum::BigInteger bnum::BigInteger::operator++(int)
{
    BigInteger tmp(*this);
    operator++();
    return tmp;
}

bnum::BigInteger& bnum::BigInteger::operator--()
{
    this->value[this->value.back()]--;
    if(this->value[this->value.back()] >= 0 ) { return *this; }
    else {
        bool done = false;
        for(auto it = this->value.rbegin(); it != this->value.rend() && !done; ++it) {
            if(*it < 0 ) {
                *it = 9;
                *(it+1)--;
            } else { done = true; }
        }
    }
    return *this;
}

bnum::BigInteger bnum::BigInteger::operator--(int)
{
    BigInteger tmp(*this);
    operator--();
    return tmp;
}

bnum::BigInteger bnum::BigInteger::operator-(BigInteger bint)
{
    return BigInteger((bint.sign == '+' ? '-' : '+'), bint.value);
}

bnum::BigInteger& bnum::BigInteger::operator+=(const bnum::BigInteger& rhs)
{
    //TODO: finish this thing
    enum class Bigger : bool {
        LHS = true,
        RHS = false
    };
    Bigger whichOne = (*this).value.size() < rhs.value.size() ? Bigger::RHS : Bigger::LHS;
    size_t i=0, j=0;
    if((*this).sign == '+' && rhs.sign == '+') {
        for(i = (*this).value.size(), j = rhs.value.size(); i>0 && j>0 && i==j; --i, --j) {
            this->value[i] += rhs.value[j];
            if((*this).value[i] > 9 ) {
                this->value[i+1]++; //= this->value[i+1] + 1;
                (*this).value[i] %=10;
            }
        }
        if(this->value[i] > 9) {
            digit_t tmp = this->value[0];
            this->value.insert(this->value.begin(), tmp/10);
            *(this->value.begin()+1) = tmp%10;
        }
    } else if((*this).sign == '+' && rhs.sign == '-') {
        for(i = (*this).value.size(), j = rhs.value.size(); i>0 && j>0 && i==j; --i, --j) {
            (*this).value[i] -= rhs.value[j];
            if((*this).value[i] < 0 ) {
                (*this).value[i+1]--;
                (*this).value[i] = 10 + (*this).value[i]; //positive + negative = subtraction 
            }
        }
        
    }
    /*switch(whichOne) {
        case Bigger::LHS : {
            return *this;
        }
        case Bigger::RHS : {
            for(/*whatever j is; j>=0; --j) {
                this->value.insert(this->value.begin(), rhs.value[j]);
            }
            return *this;
        }
    }*/
}

bnum::BigInteger& bnum::BigInteger::operator-=(const bnum::BigInteger& rhs)
{
    if((*this).isZero())
    {
        if(rhs.sign == '-'){ this->sign = '+'; }
        else { this->sign = '-'; }
        this->value = rhs.value;
        return *this;
    } else {
        if(*this < rhs) { this->sign = (this->sign == '-' && rhs.sign == '-' ? '-' : '+'); } 
    }
}

bnum::BigInteger& bnum::BigInteger::operator*=(const bnum::BigInteger& rhs)
{
    if((*this).sign == rhs.sign) { (*this).sign = '+'; }
    else { (*this).sign = '-'; }

    bool rhsMatters = false; 
    if(rhs.value.size() > (*this).value.size()) { 
        (*this).value.resize(rhs.value.size());
        rhsMatters = true;
    }

    if((*this).value.size() >= rhs.value.size()) { helpMult_((*this).value, rhs.value); }
    else {
        std::vector<digit_t> tmp = rhs.value;
        helpMult_(tmp, (*this).value);
        (*this).value = tmp;
    }
}
/*********NON-MEMBER*******/

std::istream& bnum::operator>>(std::istream& is, bnum::BigInteger& bint)
{
    std::string tmp;
    is >> tmp;
    bint = BigInteger(tmp);
    return is;
}

std::ostream& bnum::operator<<(std::ostream& os, bnum::BigInteger& bint)
{
    return os << bint.str();
}

/*** IMPLEMENTATION OF SOME <cmath> FUNCTIONS***/

bnum::BigInteger bnum::abs(bnum::BigInteger &bint)
{
    bnum::BigInteger ret;
    ret.sign = '+';
    ret.value = bint.value;
}

/*** CONVERSION ***/    

std::string bnum::BigInteger::str()
{
    std::stringstream tmp;
    tmp << this->sign;
    for(digit_t digit : this->value) {
        tmp << digit;
    }
    return tmp.str();
}

/*** UTIL ***/

bnum::digit_t bnum::BigInteger::digitAt(size_t pos, bool signIndexed)
{
    return (signIndexed ? this->value.at(pos + 1) : this->value.at(pos));
}

bnum::sign_t bnum::BigInteger::getSign()
{
    return this->sign;
}

size_t bnum::BigInteger::getLength()
{
    return this->value.size();
}

bool bnum::BigInteger::isZero()
{
    for(auto it = this->value.begin(); it != this->value.end(); ++it) {
        if(*it != 0) { return false; }
    }
    return true;
}