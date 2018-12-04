#ifndef BIGINTEGER_HPP
#define BIGINTEGER_HPP

#include <iostream>
#include <vector>
#include <string>

#include "commons.hpp"

namespace bnum 
{

    class BigInteger 
    {
        private:
            sign_t sign;
            std::vector<digit_t> value;
            void setSign(sign_t sign_);
            BigInteger(sign_t sign_, std::vector<digit_t> value_);
            void helpMult_(std::vector<digit_t> &bigger, const std::vector<digit_t> &smaller);
        public:
            /*** Constructors & Deconstructor ***/
            BigInteger();
            BigInteger(const std::string &bigIntInput);
            BigInteger(const long long int &bigIntInput);
            BigInteger(const BigInteger &bigint);

            ~BigInteger() = default;

            /*** Swap ***/
            friend void swap(BigInteger& first, BigInteger& second) {
                using std::swap;
                swap(first.sign, second.sign);
                swap(first.value, second.value);
            }

            /*** Member operator overloads ***/
            BigInteger& operator =(BigInteger rhs);
            
            BigInteger& operator++();
            BigInteger  operator++(int);
            BigInteger& operator--();
            BigInteger  operator--(int);

            BigInteger  operator -(BigInteger bint); //No need for unary plus tbh

            BigInteger& operator+=(const BigInteger& rhs);
            BigInteger& operator-=(const BigInteger& rhs);
            BigInteger& operator*=(const BigInteger& rhs);
            BigInteger& operator/=(const BigInteger& rhs);
            BigInteger& operator%=(const BigInteger& rhs);

            /*** Friend operator overlods ***/ 

            friend inline bool operator==(const BigInteger& lhs, const BigInteger& rhs) {
                return (lhs.sign == rhs.sign && lhs.value == rhs.value);
            }

            friend inline bool operator<(const BigInteger& lhs, const BigInteger& rhs) {
                if(lhs.sign == '-' && rhs.sign == '+') { return true; }
                else if(lhs.sign == '+' && rhs.sign == '-') { return false; }
                else if(lhs.value < rhs.value || lhs.value.size() < lhs.value.size() { return true; }
                else { return false; }
            }

            /*** Nice conversion functions ***/
            std::string str();
            long long int toLlInt();
            unsigned long long int toUllInt();

            /*** Util functions ***/
            digit_t digitAt(size_t pos, bool signedIndexed = true);
            sign_t getSign();
            size_t getLength();
            bool isZero();

            /*** Friend functions for different stuff ***/
            friend BigInteger abs(BigInteger &bint);
            friend BigInteger pow(BigInteger &bint);
            friend BigInteger sqrt(BigInteger &bint);
    };

    /*** Literals ***/
    inline BigInteger operator"" _bint(unsigned long long int n) {
        return BigInteger(n);
    }

    /*** Non-member operator overloads ***/
    inline bool operator!=(const BigInteger& lhs, const BigInteger& rhs) {
        return !operator==(lhs, rhs);
    }
    inline bool operator> (const BigInteger& lhs, const BigInteger& rhs) {
        return operator<(rhs, lhs);   
    }
    inline bool operator<= (const BigInteger& lhs, const BigInteger& rhs) {
        return !operator>(lhs, rhs);
    }
    inline bool operator>= (const BigInteger& lhs, const BigInteger& rhs) {
        return !operator<(lhs, rhs);
    }

    inline BigInteger operator+(BigInteger lhs, BigInteger& rhs) {
        lhs+=rhs;
        return lhs;
    }
    inline BigInteger operator-(BigInteger lhs, BigInteger& rhs) {
        lhs-=rhs;
        return lhs;
    }
    inline BigInteger operator*(BigInteger lhs, BigInteger& rhs) {
        lhs*=rhs;
        return lhs;
    }
    inline BigInteger operator/(BigInteger lhs, BigInteger& rhs) {
        lhs/=rhs;
        return lhs;
    }
    inline BigInteger operator%(BigInteger lhs, BigInteger& rhs) {
        lhs%=rhs;
        return lhs;
    }

    std::ostream& operator<<(std::ostream& os, const BigInteger& rhs);
    std::istream& operator>>(std::istream& is, const BigInteger& rhs);
}

#endif //BIGINTEGER_HPP