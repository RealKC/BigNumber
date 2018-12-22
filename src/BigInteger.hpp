#ifndef BIGINTEGER_HPP
#define BIGINTEGER_HPP

#include <iostream>
#include <vector>
#include <string>

namespace bnum 
{
    class BigInteger {
        public:
            using digit_t = short;
            enum class sign_t : char {
                plus = '+',
                minus = '-'
            };
        private:
            sign_t sign;
            std::vector<digit_t> value;
            
        public:
            /* Constructors & Deconstructor */
            BigInteger();
            BigInteger(const std::string bigIntInput);
            BigInteger(const long long int bigIntInput);
            BigInteger(const BigInteger &bigint);

            ~BigInteger() = default;

            /*** Swap ***/
            friend void swap(BigInteger& first, BigInteger& second);

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
                if(lhs.sign == sign_t::MINUS && rhs.sign == sign_t::PLUS) { return true; }
                else if(lhs.sign == sign_t::PLUS && rhs.sign == sign_t::MINUS) { return false; }
                else if(lhs.value < rhs.value || lhs.value.size() < lhs.value.size()) { return true; }
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
            bool isZero() const;

            /*** Friend functions for different stuff ***/
            friend BigInteger abs(BigInteger &bint);
            friend BigInteger pow(BigInteger &bint);
            friend BigInteger sqrt(BigInteger &bint);

        private:
            BigInteger(sign_t sign_, std::vector<digit_t> value_);
            void _helpMult(std::vector<digit_t> &bigger, const std::vector<digit_t> &smaller);
            std::vector<digit_t>& _helpAdd(const std::vector<digit_t> &lhs, const std::vector<digit_t> &rhs);
            std::vector<digit_t>& _helpSub(const std::vector<digit_t> &lhs, const std::vector<digit_t> &rhs);
    };

    void swap(BigInteger&, BigInteger&); 
    BigInteger abs(BigInteger &bint);
    BigInteger pow(BigInteger &bint);
    BigInteger sqrt(BigInteger &bint);

    /*** Literals ***/
    inline BigInteger operator"" _bint(unsigned long long int n) {
        return BigInteger(n);
    }

    /*** Non-member operator overloads ***/

    std::ostream& operator<<(std::ostream& os, BigInteger& bint);
    std::istream& operator>>(std::istream& is, BigInteger& bint);

    inline bool operator!=(const BigInteger& lhs, const BigInteger& rhs) {
        return !operator==(lhs, rhs);
    }
    inline bool operator> (const BigInteger& lhs, const BigInteger& rhs) {
        return operator<(rhs, lhs);   
    }
    inline bool operator<=(const BigInteger& lhs, const BigInteger& rhs) {
        return !operator>(lhs, rhs);
    }
    inline bool operator>=(const BigInteger& lhs, const BigInteger& rhs) {
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
}

#endif //BIGINTEGER_HPP