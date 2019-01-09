#ifndef BNUM_EXCEPTIONS_HPP
#define BNUM_EXCEPTIONS_HPP

#include <exception>
#include <string>

namespace BigNumbers
{
namespace Exceptions
{
    /*Exception thrown by bnum::BigInteger::digitAt() */
    class NonexistantDigit : public std::exception {
    public:
        explicit NonexistantDigit(const std::string& what_arg);
        explicit NonexistantDigit(const char* what_arg);
        NonexistantDigit(const NonexistantDigit& arg);

        NonexistantDigit& operator=(const NonexistantDigit& lhs) noexcept;
        
        virtual ~NonexistantDigit();

        virtual const char* what() noexcept;

    private:
        char* what_message; 
    };

    class DivisionByZero : public std::exception {
    public:
        explicit DivisionByZero(const std::string& what_arg);
        explicit DivisionByZero(const char* what_arg);
        DivisionByZero(const DivisionByZero& arg);

        DivisionByZero& operator=(const DivisionByZero& lhs) noexcept;

        virtual ~DivisionByZero();

        virtual const char* what() noexcept;

    private:
        char* what_message;
    };
}//Exception
}//BigNumbers

#endif //BNUM_EXCEPTIONS_HPP
