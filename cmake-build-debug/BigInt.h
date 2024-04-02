#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include "InvalidInputException.h"

class DivideByZeroException : public std::exception {
public:
    const char *what() const noexcept override {
        return "Division by zero";
    }
};

class InvalidExponent : public std::exception {
public:
    const char *what() const noexcept override {
        return "Invalid exponent";
    }
};


class BigInt {
private:
    std::string number;
    bool positive;

public:
    // Constructors
    BigInt() ;

    BigInt(const std::string &num) ;
    BigInt(int num) ;

    void remove_leading_zeroes();

    std::string to_string() const;

    static std::string module(std::string str);

    // Math operators
    BigInt operator+(const BigInt &other) const;

    BigInt &operator+=(const BigInt &other);

    BigInt operator-(const BigInt &other) const;

    BigInt &operator-=(const BigInt &other);

    BigInt operator*(const BigInt &other) const;

    BigInt &operator*=(const BigInt &other);

    BigInt operator/(const BigInt &other) const;

    BigInt &operator/=(const BigInt &other);

    BigInt operator%(const BigInt &other) const;

    BigInt &operator%=(const BigInt &other);

    //Integer
    BigInt operator+(int other);

    BigInt &operator+=(int other);

    BigInt operator-(int other);

    BigInt &operator-=(int other);

    BigInt operator*(int other) ;

    BigInt &operator*=(int other);

    BigInt operator/(int other) ;

    BigInt &operator/=(int other);

    BigInt operator%(int other);

    BigInt &operator%=(int other);

    // Compare operators
    bool compare(std::string other_num) const;

    bool operator==(const BigInt &other) const;

    bool operator!=(const BigInt &other) const;

    bool operator<(const BigInt &other) const;

    bool operator<=(const BigInt &other) const;

    bool operator>(const BigInt &other) const;

    bool operator>=(const BigInt &other) const;

    //Std iostream
    friend std::ostream &operator<<(std::ostream &os, const BigInt &num);

    friend std::istream &operator>>(std::istream &is, BigInt &num);

    BigInt pow(int exponent);

    static BigInt factorial(int n);

    BigInt gcd(const BigInt &other);

    BigInt operator++(int);//postfix bigint

    BigInt operator--(int);//postfix bigint

    BigInt operator-() ;//unary minus

    BigInt operator+() ;//unary plus

    BigInt &operator++();//prefix bigint

    BigInt &operator--();//prefix bigint

    friend BigInt operator+(const BigInt &a, const int &b);

    friend BigInt operator+(const int &a, const BigInt &b);

    static BigInt karatsubaMultiply(const BigInt &X, const BigInt &Y);

private:
    std::string add(std::string second, std::string first) const;

    std::string substraction(std::string second, std::string first) const;

    std::string multiply(std::string other) const;
    std::string divide(const BigInt &num, const BigInt &denom) const;
};