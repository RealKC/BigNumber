#include <cstring>
#include <new>

#include "exceptions.hpp"

namespace BigNumbers::Exceptions
{
    /*** NonExistantDigit ***/

    NonexistantDigit::NonexistantDigit(const std::string& what_arg) {
        this->what_message = new char[std::strlen(what_arg.c_str()) + 1];
        std::strcpy(this->what_message, what_arg.c_str());
    }

    NonexistantDigit::NonexistantDigit(const char* what_arg) {
        this->what_message = new char[std::strlen(what_arg) + 1];
        std::strcpy(this->what_message, what_arg);
    }

    NonexistantDigit::NonexistantDigit(const NonexistantDigit& arg) {
        this->what_message = new char[std::strlen(arg.what_message) + 1];
        std::strcpy(this->what_message, arg.what_message);
    }

    NonexistantDigit& NonexistantDigit::operator=(const NonexistantDigit& that) {
        char* tmp = new char[std::strlen(that.what_message) + 1];
        strcpy(tmp, that.what_message);
        delete [] what_message;
        what_message = tmp;
        return *this;
    }

    NonexistantDigit::~NonexistantDigit() {
        delete [] what_message;
    }

    const char* NonexistantDigit::what() {
        return what_message;
    }


    /*** DivisionByZero ***/

    DivisionByZero::DivisionByZero(const std::string& what_arg) {
        this->what_message = new char(std::strlen(what_arg.c_str()) + 1);
        std::strcpy(this->what_message, what_arg.c_str());
    }

    DivisionByZero::DivisionByZero(const char* what_arg) {
        this->what_message = new char(strlen(what_arg) + 1);
        std::strcpy(this->what_message, what_arg);
    }

    DivisionByZero::DivisionByZero(const DivisionByZero& arg) {
        this->what_message = new char(strlen(arg.what_message) + 1);
        std::strcpy(this->what_message, arg.what_message);
    }

    DivisionByZero& DivisionByZero::operator=(const DivisionByZero& that) {
        char* tmp = new char[std::strlen(that.what_message) + 1];
        strcpy(tmp, that.what_message);
        delete [] what_message;
        what_message = tmp;
        return *this;
    }

    DivisionByZero::~DivisionByZero() {
        delete this->what_message;
    }

    const char* DivisionByZero::what() {
        return this->what_message;
    }

}
