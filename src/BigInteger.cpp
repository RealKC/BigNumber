#include <algorithm>
#include <cctype>
#include <string>
#include <sstream>
#include <vector>

#include "BigInteger.hpp"

namespace bnum
{

/*** CONSTRUCTORS ***/

BigInteger::BigInteger() {}

BigInteger::BigInteger(const std::string bigIntInput)
{
    bool startFromOne;
    if(!isalpha(bigIntInput[0]) && (bigIntInput[0]=='+' || bigIntInput[0]=='-')) {
        this->sign=static_cast<sign_t>(bigIntInput[0]);
        startFromOne=true;
    } else { 
        this->sign=sign_t::plus;
        startFromOne=false;
    }

    for(size_t i = (startFromOne ? 1 : 0); i<bigIntInput.size(); ++i) {
        this->value.push_back(bigIntInput[i]-'0');
    }
}

BigInteger::BigInteger(const long long int bigIntInput)
{
    long long int aux = bigIntInput;
    if(bigIntInput >= 0) { this->sign=sign_t::plus; }
    else { 
        this->sign=sign_t::minus;
        aux = -aux;
    }

    while(aux!=0) {
        this->value.push_back(static_cast<BigInteger::digit_t>(aux%10));
        aux/=10;
    }

    std::reverse(this->value.begin(), this->value.end());
}

BigInteger::BigInteger(sign_t sign_, std::vector<digit_t> value_) 
{
    sign = sign_;
    this->value.resize(value_.size());
    value = value_;   
}

BigInteger::BigInteger(const BigInteger& bint)
{
    this->sign=bint.sign;
    this->value=bint.value;
}

/*** PRIVATE HELPERS ***/

void BigInteger::_helpMult(std::vector<BigInteger::digit_t> &bigger, const std::vector<BigInteger::digit_t> &smaller)
{
    size_t i, j;
    for(i = smaller.size()-1; i>=0; --i) {
        for(j = bigger.size()-1; j>0; --j) {
            bigger[j] *= smaller[i];
            if(bigger[j] > 9) {
                bigger[j-1] += bigger[j] % 10;
                bigger[j] /= 10;
            }
        }
        if(bigger[0] > 10) {
            BigInteger::digit_t tmp = bigger[0];
            bigger[0] %= 10;
            bigger.insert(bigger.begin(), tmp/10);
        }
    }
}

std::vector<BigInteger::digit_t>& BigInteger::_helpAdd(const std::vector<digit_t> &lhs, const std::vector<digit_t> &rhs)
{
    digit_t carry = 0;
    std::vector<digit_t> sum;
    sum.reserve(lhs.size() > rhs.size() ? lhs.size() : rhs.size());
    auto it_sum_digit = sum.rbegin();
    auto i = lhs.rbegin(), j = rhs.rbegin();

    for(; i != lhs.rend() && j != rhs.rend(); ++i, ++j) {
        (*it_sum_digit) = carry + (*i) + (*j);
        if((*it_sum_digit) > 9) {
            carry = 1;
            (*it_sum_digit) %= 10;
        } else { carry = 0; }
        ++it_sum_digit;
    }

    if(i != lhs.rend()) {
        //lhs and sum should have the same size if we get in this if
        if(carry) {
            (*it_sum_digit) = (*i) + carry;
            ++i; ++it_sum_digit;
        }
        for(; i != lhs.rend() && it_sum_digit != sum.rend(); ++i, ++it_sum_digit) {
            (*it_sum_digit) = (*i);
        }
    }
    if(j != rhs.rend()) {
        //lhs and sum should have the same size if we get in this if
        if(carry) {
            (*it_sum_digit) = (*j) + carry;
            ++j; ++it_sum_digit;
        }
        for(;j != rhs.rend() && it_sum_digit != sum.rend(); ++j, ++it_sum_digit) {
            (*it_sum_digit) = (*j);
        }
    }

    return sum;
}

std::vector<BigInteger::digit_t>& BigInteger::_helpSub(const std::vector<digit_t> &lhs, const std::vector<digit_t> &rhs) {
    std::vector<digit_t> difference(lhs.size() > rhs.size() ? lhs.size() : rhs.size(), 0);
    digit_t carry = 0;
    auto it_diff_digit = difference.rbegin();
    auto i = lhs.rbegin(), j = rhs.rbegin();

    for(; i != lhs.rend() && j != rhs.rend(); ++i, ++j, ++it_diff_digit) {
        (*it_diff_digit) = (*i) - (*j) + carry;
        if((*it_diff_digit) < 0) {
            carry = -1;
        } else { carry = 0; }
    }

    if(i != lhs.rend()) {
        if(carry == -1) {
            *it_diff_digit = *i + carry;
            ++i; ++it_diff_digit;
        }
        for(; i != lhs.rend(), it_diff_digit != difference.rend(); ++i, ++it_diff_digit) {
            *it_diff_digit = *i;
        }
    }
    if(j != rhs.rend()) {
        if(carry == -1) {
            *it_diff_digit = *j + carry;
            ++j; ++it_diff_digit;
        }
        for(; j != rhs.rend(), it_diff_digit != difference.rend(); ++j, ++it_diff_digit) {
            *it_diff_digit = *j;
        }
    }

    if(difference[0] == 0){
       difference.erase(difference.begin(),
                        std::find(difference.begin(), difference.end(),
                                [](auto elem){ return elem != 0; }));
    }
    return difference;
}
/*** OPERATOR OVERLOADS ***/

/***********MEMBER*********/
BigInteger& BigInteger::operator=(BigInteger rhs)
{
    swap(*this, rhs);
    return *this;
}

BigInteger& BigInteger::operator++()
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
    if(*(this->value.begin()) > 9) {
        BigInteger::digit_t tmp=value[0];
        this->value.insert(this->value.begin(), tmp/10);
        *(this->value.begin()+1)=tmp%10;
    }
    return *this;
}

BigInteger BigInteger::operator++(int)
{
    BigInteger tmp(*this);
    operator++();
    return tmp;
}

BigInteger& BigInteger::operator--()
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

BigInteger BigInteger::operator--(int)
{
    BigInteger tmp(*this);
    operator--();
    return tmp;
}

BigInteger BigInteger::operator-(BigInteger bint)
{
    return BigInteger(bint.sign == sign_t::plus ? sign_t::minus : sign_t::plus, bint.value);
}

BigInteger& BigInteger::operator+=(const BigInteger& rhs)
{
    if((*this).isZero()) {
        this->sign = rhs.sign;
        this->value.resize(rhs.value.size());
        this->value = rhs.value;
    } else if(rhs.isZero()) { return *this; } //No reason to do anything in this case
    else if(this->sign == rhs.sign) {
        this->value = _helpAdd(this->value, rhs.value);
    } else { //differing signs, which is when addition is really subtraction
        if(*this > rhs) {
            this->value = _helpSub(this->value, rhs.value);
        } else {
            this->value = _helpSub(rhs.value, this->value);
        }
    }

    return *this;
}

BigInteger& BigInteger::operator-=(const BigInteger& rhs)
{
    if(rhs.isZero()) { return *this; } //Why do anything in this case anyway?
    else if((*this).isZero()) {
        this->sign = (rhs.sign == sign_t::plus) ? sign_t::minus : sign_t::plus;
        this->value.resize(rhs.value.size());
        this->value = rhs.value;
        return *this;
    } else if(this->sign == sign_t::minus && rhs.sign == sign_t::plus) {
        //Subtracting a positive from a negative makes addition
        //Example: (-5)-(3) = -8
        this->value = _helpAdd(this->value, rhs.value);
    } else {
        if(rhs > *this) {
            this->value = _helpSub(rhs.value, this->value);
        } else {
            this->value = _helpSub(this->value, rhs.value);
        }
    }
    
    return *this;
}

BigInteger& BigInteger::operator*=(const BigInteger& rhs)
{
    if((*this).isZero() || rhs.isZero() ) {
        this->sign = sign_t::plus;
        this->value = { 0 };
        return *this;
    }
    
    if((*this).sign == rhs.sign) { (*this).sign = sign_t::plus; }
    else { (*this).sign = sign_t::minus; }
/*
    bool rhsMatters = false; 
    if(rhs.value.size() > (*this).value.size()) { 
        (*this).value.resize(rhs.value.size());
        rhsMatters = true;
    }

    if((*this).value.size() >= rhs.value.size()) { _helpMult((*this).value, rhs.value); }
    else {
        std::vector<BigInteger::digit_t> tmp = rhs.value;
        _helpMult(tmp, (*this).value);
        (*this).value = tmp;
    }*/
}
/*********NON-MEMBER*******/

std::istream& operator>>(std::istream& is, BigInteger& bint)
{
    std::string tmp;
    is >> tmp;
    bint = BigInteger(tmp);
    return is;
}

std::ostream& operator<<(std::ostream& os, BigInteger& bint)
{
    os << bint.str();
    return os;
}

/*** IMPLEMENTATION OF SOME <cmath> FUNCTIONS***/

BigInteger abs(BigInteger &bint)
{
    BigInteger ret;
    ret.sign = BigInteger::sign_t::plus;
    ret.value = bint.value;
    return ret;
}

/*** CONVERSION ***/    

std::string BigInteger::str()
{
    std::stringstream tmp;
    tmp << static_cast<char>(this->sign);
    for(BigInteger::digit_t digit : this->value) {
        tmp << digit;
    }
    return tmp.str();
}

/*** UTIL ***/

BigInteger::digit_t BigInteger::digitAt(size_t pos, bool signIndexed)
{
    return (signIndexed ? this->value.at(pos + 1) : this->value.at(pos));
}

BigInteger::sign_t BigInteger::getSign()
{
    return this->sign;
}

size_t BigInteger::getLength()
{
    return this->value.size();
}

bool BigInteger::isZero() const
{
    for(auto it = this->value.begin(); it != this->value.end(); ++it) {
        if(*it != 0) { return false; }
    }
    return true;
}

/*** FRIENDS ***/

void swap(BigInteger& first, BigInteger& second){
    using std::swap;
    swap(first.sign, second.sign);
    swap(first.value, second.value);
}

}