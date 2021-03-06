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

BigInteger::BigInteger(const unsigned long long int bigIntInput)
{
    unsigned long long int aux = bigIntInput;
    this->sign=sign_t::plus;

    while(aux!=0) {
        this->value.push_back(static_cast<BigInteger::digit_t>(aux%10));
        aux/=10;
    }
}
BigInteger::BigInteger(const unsigned long int bigIntInput)
{
    unsigned long int aux = bigIntInput;
    this->sign=sign_t::plus;

    while(aux!=0) {
        this->value.push_back(static_cast<BigInteger::digit_t>(aux%10));
        aux/=10;
    }
}
BigInteger::BigInteger(const unsigned int bigIntInput)
{
    unsigned int aux = bigIntInput;
    this->sign=sign_t::plus;

    while(aux!=0) {
        this->value.push_back(static_cast<BigInteger::digit_t>(aux%10));
        aux/=10;
    }
}
BigInteger::BigInteger(const unsigned short bigIntInput)
{
    unsigned short aux = bigIntInput;
    this->sign=sign_t::plus;

    while(aux!=0) {
        this->value.push_back(static_cast<BigInteger::digit_t>(aux%10));
        aux/=10;
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

BigInteger::BigInteger(const long int bigIntInput)
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

BigInteger::BigInteger(const int bigIntInput)
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

    for(size_t i = 0; i < lhs.size(); i++) {
        for(size_t j = 0; j < rhs.size(); j++) {
            product[i+j] += lhs[i] * rhs[i];
        }
    }

    for(size_t i = 0; i <= product.size(); i++) {
        if(product[i] > 9) {
            product[i+1] += product[i] % 10;
            product[i] /= 10;
        }
    }

    if(*(product.end()- 1) == 0 ){
        product.erase(product.end() - 1);
        product.shrink_to_fit();
    }

    return product;
}

std::pair<BigInteger, BigInteger>& BigInteger::_helpDiv(BigInteger &lhs, const BigInteger &rhs)
{
    std::pair<BigInteger, BigInteger> quotientRemainderPair;
    while(!lhs.isZero() && lhs > rhs) {
        lhs-=rhs;
        ++quotientRemainderPair.first;
    }
    quotientRemainderPair.second = lhs;
    
    return quotientRemainderPair;
}

std::vector<BigInteger::digit_t>& BigInteger::_helpAdd(const std::vector<digit_t> &lhs, const std::vector<digit_t> &rhs)
{
    digit_t carry = 0;
    std::vector<digit_t> sum;
    sum.resize(lhs.size() > rhs.size() ? lhs.size() : rhs.size());
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

    if(this->sign == rhs.sign) { this->sign = sign_t::plus; }
    else { this->sign = sign_t::minus; }

    this->value = _helpMult(this->value, rhs.value);

    return *this;
}

BigInteger& BigInteger::operator/=(const BigInteger& rhs) 
{
    if(rhs.isZero()) {
        throw Exceptions::DivisionByZero("division by zero");
    }
    BigInteger tmp = BigInteger(sign_t::plus, this->value);
    this->value = _helpDiv(tmp, BigInteger(sign_t::plus, rhs.value)).first.value;

    this->sign = (this->sign == rhs.sign) ? sign_t::plus : sign_t::minus;

    return *this;
}

BigInteger& BigInteger::operator%=(const BigInteger& rhs) 
{
    if(rhs.isZero()) {
        throw Exceptions::DivisionByZero("division by zero");
    }
    BigInteger tmp = BigInteger(sign_t::plus, this->value);
    this->value = _helpDiv(tmp, BigInteger(sign_t::plus, rhs.value)).second.value;

    return *this;
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

/*** IMPLEMENTATION OF SOME MATHEMATICAL FUNCTIONS***/

BigInteger abs(const BigInteger &bint)
{
    BigInteger ret;
    ret.sign = BigInteger::sign_t::plus;
    ret.value = bint.value;
    return ret;
}

BigInteger pow(BigInteger &base, BigInteger& exponent) 
{
    if(base.isZero()) {
        return 0_BigInt;
    }
    BigInteger result = 1;
    for(BigInteger i = 0; i < exponent; ++i) {
        result *= base;
    }

    return result;
}

BigInteger sqrt(BigInteger& bint)
{
    if(bint < 0) {
        throw Exceptions::NegativeArgument("square root of a negative does not exist");
    }

    //Wrote using this Wikipedia page https://en.wikipedia.org/wiki/Integer_square_root#Using_only_integer_division
    // and this SO post https://stackoverflow.com/questions/21657491/an-efficient-algorithm-to-calculate-the-integer-square-root-isqrt-of-arbitrari
    // despite it being tagged for erlang, it contained a C/C++ answer
    if(bint.isZero()) {
        return 0_BigInt;
    }

    BigInteger xk = bint, xk1 = 0;
    do {
        xk1 = (xk + bint / xk) / 2_BigInt;
        if(xk1 >= xk) {
            return xk;
        } else {
            xk = xk1;
        }
    } while(true);
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
        throw Exceptions::NonexistantDigit("pos does not indicate a digit that exists");
    }
}

BigInteger::sign_t BigInteger::getSign()
{
    return this->sign;
}

size_t BigInteger::getNumberOfDigits()
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

void swap(BigInteger& first, BigInteger& second)
{
    using std::swap;
    swap(first.sign, second.sign);
    swap(first.value, second.value);
}
}
