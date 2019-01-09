#include <algorithm>
#include <cctype>
#include <string>
#include <sstream>
#include <vector>

#include "BigInteger.hpp"

namespace BigNumbers
{

/*** CONSTRUCTORS ***/

BigInteger::BigInteger() 
    : sign(sign_t::plus)
    , value(1, 0) 
    {}

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

    for(size_t i = static_cast<size_t>(startFromOne); i<bigIntInput.size(); ++i) {
        this->value.insert(this->value.begin(), static_cast<digit_t>(bigIntInput[i] - '0'));
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
}

BigInteger::BigInteger(sign_t sign_, std::vector<digit_t> value_) 
{
    sign = sign_;
    this->value.resize(value_.size());
    value = value_;   
}

BigInteger::BigInteger(const BigInteger &bigIntInput)
{
    this->sign=bigIntInput.sign;
    this->value=bigIntInput.value;
}

BigInteger::BigInteger(BigInteger &&bigIntInput) 
    : sign(std::move(bigIntInput.sign))
    , value(std::move(bigIntInput.value)) 
    {}

/*** PRIVATE HELPERS ***/

std::vector<BigInteger::digit_t>& BigInteger::_helpMult(const std::vector<BigInteger::digit_t> &lhs, const std::vector<BigInteger::digit_t> &rhs)
{
    digit_t carry;
    std::vector<digit_t> product;
    product.resize(lhs.size() + rhs.size());

    for(auto i = lhs.size() - 1; i >= 0; --i) {
        auto it_prod_digit = product.size() - 1;
        carry = 0;
        for(auto j = rhs.size() - 1; j >= 0; --j, --it_prod_digit) {
            product[i+j] += carry + lhs[i] * rhs[j];
            carry = product[i+j] / 10;
            product[i+j] %= 10;
        }
        product[i];
    }
}

std::vector<BigInteger::digit_t>& BigInteger::_helpAdd(const std::vector<digit_t> &lhs, const std::vector<digit_t> &rhs)
{
    digit_t carry = 0;
    std::vector<digit_t> sum;
    sum.reserve(lhs.size() > rhs.size() ? lhs.size() : rhs.size());
    auto it_sum_digit = sum.begin();
    auto i = lhs.begin(), j = rhs.begin();

    for(; i != lhs.end() && j != rhs.end(); ++i, ++j) {
        (*it_sum_digit) = carry + (*i) + (*j);
        if((*it_sum_digit) > 9) {
            carry = 1;
            (*it_sum_digit) %= 10;
        } else { carry = 0; }
        ++it_sum_digit;
    }

    if(i != lhs.end()) {
        //lhs and sum should have the same size if we get in this if
        if(carry) {
            (*it_sum_digit) = (*i) + carry;
            ++i; ++it_sum_digit;
        }
        for(; i != lhs.end() && it_sum_digit != sum.end(); ++i, ++it_sum_digit) {
            (*it_sum_digit) = (*i);
        }
    }
    if(j != rhs.end()) {
        //lhs and sum should have the same size if we get in this if
        if(carry) {
            (*it_sum_digit) = (*j) + carry;
            ++j; ++it_sum_digit;
        }
        for(;j != rhs.end() && it_sum_digit != sum.end(); ++j, ++it_sum_digit) {
            (*it_sum_digit) = (*j);
        }
    }

    return sum;
}

std::vector<BigInteger::digit_t>& BigInteger::_helpSub(const std::vector<digit_t> &lhs, const std::vector<digit_t> &rhs) {
    std::vector<digit_t> difference(lhs.size() > rhs.size() ? lhs.size() : rhs.size(), 0);
    digit_t carry = 0;
    auto it_diff_digit = difference.begin();
    auto i = lhs.begin(), j = rhs.begin();

    for(; i != lhs.end() && j != rhs.end(); ++i, ++j, ++it_diff_digit) {
        (*it_diff_digit) = (*i) - (*j) + carry;
        if((*it_diff_digit) < 0) {
            carry = -1;
        } else { carry = 0; }
    }

    if(i != lhs.end()) {
        if(carry == -1) {
            *it_diff_digit = *i + carry;
            ++i; ++it_diff_digit;
        }
        for(; i != lhs.end(), it_diff_digit != difference.end(); ++i, ++it_diff_digit) {
            *it_diff_digit = *i;
        }
    }
    if(j != rhs.end()) {
        if(carry == -1) {
            *it_diff_digit = *j + carry;
            ++j; ++it_diff_digit;
        }
        for(; j != rhs.end(), it_diff_digit != difference.end(); ++j, ++it_diff_digit) {
            *it_diff_digit = *j;
        }
    }

    if(*(difference.end() - 1) == 0){
        difference.erase(std::find(difference.rbegin(), difference.rend(),
                                [](digit_t elem){ return elem != 0; }).base(),
                        (++(difference.rbegin())).base());
        difference.shrink_to_fit();
    }
    return difference;
}
/*** OPERATOR OVERLOADS ***/

/***********MEMBER*********/
BigInteger& BigInteger::operator=(BigInteger &rhs)
{
    this->sign = rhs.sign;
    this->value = rhs.value;
    return *this;
}

BigInteger& BigInteger::operator=(BigInteger &&rhs)
{
    this->sign = std::move(rhs.sign);
    this->value = std::move(rhs.value);
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
    bint = tmp;
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
    for(auto it = this->value.rbegin(), it_end = this->value.rend(); it != it_end; it++) {
        tmp << (*it);
    }
    return tmp.str();
}

/*** UTIL ***/

BigInteger::digit_t BigInteger::digitAt(size_t pos)
{
    try {
        return this->value.at(this->value.size() - pos);
    } catch(std::out_of_range) {
        throw Exceptions::NonexistantDigit("pos does indicate a digit that exists");
    }
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