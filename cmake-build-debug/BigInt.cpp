#include <stdexcept>
#include <cmath>
#include "BigInt.h"
// Constructors
BigInt::BigInt() {
    this->number = "0";
}

BigInt::BigInt(const std::string &num) {
    std::string n = num;

    if (num[0] == '-') {
        this->number = num.substr(1);
        this->positive = false;
        n = num.substr(1);
    } else {
        this->number = num;
        this->positive = true;
    }
    remove_leading_zeroes();

    //Check the whole number for the presence of letters
    for (int i = 0; i < number.size(); ++i) {
        if (number[i] < '0' || number[i] > '9' || number[i] == ' ') {
            // If the current character is not a digit, throw an exception
            throw InvalidInputException();
            return;
        }
    }
}

BigInt::BigInt(int num) {
    if (num < 0) {
        positive = false;
        num = -num;
    } else {
        positive = true;
    }
    number = std::to_string(num);
}
std::string BigInt::add(std::string second, std::string first) const {
    std::string result;
    int carry = 0;
    int max_length = std::max(first.size(), second.size());
    while (first.size() < max_length) first.insert(0, "0");
    while (second.size() < max_length) second.insert(0, "0");

    for (int i = max_length - 1; i >= 0; --i) {
        int digit_sum = (first[i] - '0') + (second[i] - '0') + carry;
        carry = digit_sum / 10;
        result.insert(result.begin(), (digit_sum % 10) + '0');
    }

    if (carry > 0) {
        result.insert(result.begin(), carry + '0');
    }
    return result;
}

std::string BigInt::substraction(std::string second, std::string first) const {
    // Check for empty strings
    if (first.empty() || second.empty()) {
        return "";
    }

    if (BigInt(second) > BigInt(first)) {
        std::swap(first, second);
    }

    std::vector<int> num1, num2, result;

    // Convert strings to vectors of integers, reversing them
    for (char digit: first) {
        num1.push_back(digit - '0');
    }
    std::reverse(num1.begin(), num1.end());

    for (char digit: second) {
        num2.push_back(digit - '0');
    }
    std::reverse(num2.begin(), num2.end());

    int borrow = 0;
    for (size_t i = 0; i < num1.size(); ++i) {
        int larger_digit = num1[i];
        int smaller_digit = (i < num2.size()) ? num2[i] : 0;
        int diff = larger_digit - smaller_digit - borrow;

        if (diff < 0) {
            diff += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }
        result.push_back(diff);
    }

    // Construct the result string from the vector, reversing it back
    std::string result_str;
    for (auto it = result.rbegin(); it != result.rend(); ++it) {
        result_str.push_back(*it + '0');
    }

    // Remove leading zeros from the result
    size_t non_zero_index = result_str.find_first_not_of('0');
    if (non_zero_index != std::string::npos) {
        result_str = result_str.substr(non_zero_index);
    } else {
        result_str = "0";
    }

    // Return the correct sign for the result
    return result_str;
}

std::string BigInt::multiply(std::string other) const {
    // Check for zero inputs
    if (number == "0" || other == "0") {
        return "0";
    }

    std::vector<int> result(number.size() + other.size(), 0);

    int carry = 0;

    for (int i = number.size() - 1; i >= 0; --i) {
        int digit1 = number[i] - '0';

        for (int j = other.size() - 1; j >= 0; --j) {
            int digit2 = other[j] - '0';
            int product = digit1 * digit2 + result[i + j + 1] + carry;
            carry = product / 10;
            result[i + j + 1] = product % 10;
        }
        result[i] += carry;
        carry = 0;
    }

    // Converting a vector to a string
    std::string result_string;
    for (int digit: result) {
        result_string.push_back(digit + '0');
    }

    // Removing leading zeros from the result
    size_t non_zero_index = result_string.find_first_not_of('0');
    if (non_zero_index != std::string::npos) {
        return result_string.substr(non_zero_index);
    } else {
        return "0";
    }
}

BigInt BigInt::operator+(const BigInt &other) const {
    BigInt result;

    // If both numbers are positive or both are negative
    if ((positive && other.positive) || (!positive && !other.positive)) {
        result = add(this->number, other.number);
        result.positive = positive; // The sign of the result will be positive if both numbers have the same sign
    } else {
        // If the numbers have different signs, I use the subtraction operator
        if (positive) {
            // If the first number is positive and the second is negative, subtract the second from the first
            result = substraction(this->number, other.number);
        } else {
            // If the first number is negative and the second is positive, we subtract the first from the second and change the sign of the result
            result = substraction(other.number, this->number);
        }
        //Assign a sign of a larger number
        if (BigInt(this->number) > BigInt(other.number)) {
            result.positive = this->positive;
        } else {
            result.positive = other.positive;
        }
    }

    // Removing leading zeros from the result
    result.remove_leading_zeroes();
    return result;
}

BigInt &BigInt::operator+=(const BigInt &other) {
    BigInt result;

    // If both numbers are positive or both are negative
    if ((positive && other.positive) || (!positive && !other.positive)) {
        result = add(this->number, other.number);
        result.positive = positive; // The sign of the result will be positive if both numbers have the same sign
    } else {
        // If the numbers have different signs, use the subtraction operator
        if (positive) {
            // If the first number is positive and the second is negative, subtract the second from the first
            result = substraction(this->number, other.number);
        } else {
            // If the first number is negative and the second is positive, we subtract the first from the second and change the sign of the result
            result = substraction(other.number, this->number);
        }
        //Assign a sign of a larger number
        if (BigInt(this->number) > BigInt(other.number)) {
            result.positive = this->positive;
        } else {
            result.positive = other.positive;
        }
    }

    // Removing leading zeros from the result
    result.remove_leading_zeroes();
    this->number = result.number;
    this->positive = result.positive;
    return *this;
}

BigInt BigInt::operator-(const BigInt &other) const {
    BigInt res;
    if (positive && other.positive) {
        // Positive - Positive
        if (*this >= other) {
            res.number = substraction(this->number, other.number);
            res.positive = true;
        } else {
            res.number = substraction(other.number, this->number);
            res.positive = false;
        }
    } else if (!positive && !other.positive) {
        // Negative - Negative
        if (*this >= other) {
            res.number = substraction(this->number, other.number);
            res.positive = false; // Result is negative
        } else {
            res.number = substraction(other.number, this->number);
            res.positive = true; // Result is positive
        }
    } else if (positive && !other.positive) {
        // Positive - Negative
        res.number = add(this->number, other.number);
        res.positive = true;
    } else if (!positive && other.positive) {
        // Negative - Positive
        res.number = add(this->number, other.number);
        res.positive = false;
    }
    res.remove_leading_zeroes();
    return res;
}

BigInt &BigInt::operator-=(const BigInt &other) {
    *this = *this - other;
    return *this;
}

BigInt BigInt::operator*(const BigInt &other) const {
    BigInt result_BigInt(multiply(other.number));

    // If either operand is zero, the result is zero with the correct sign
    if (this->number == "0" || other.number == "0") {
        result_BigInt.number = "0";
        result_BigInt.positive = true; // Zero is considered positive
    } else {
        result_BigInt.positive = (this->positive && other.positive) || (!this->positive && !other.positive);
    }
    return result_BigInt;
}

BigInt &BigInt::operator*=(const BigInt &other) {
    BigInt result(multiply(other.number));

    // If either operand is zero, the result is zero with the correct sign
    if (this->number == "0" || other.number == "0") {
        this->number = "0";
        this->positive = true; // Zero is considered positive
    } else {
        this->positive = (this->positive && other.positive) || (!this->positive && !other.positive);
        this->number = result.number;
    }
    return *this;
}

BigInt BigInt::operator/(const BigInt &other) const {
    if (other.number == "0") {
        throw DivideByZeroException();
    }

    BigInt quotient("0");
    BigInt remainder = *this;
    remainder.positive = true;

    BigInt divisor = other;
    divisor.positive = true;

    if (remainder < divisor) {
        return BigInt(0);
    }

    std::string partial_dividend;

    for (size_t i = 0; i < remainder.number.size(); ++i) {
        partial_dividend += remainder.number[i];
        BigInt partial_BigInt(partial_dividend);

        int quotient_digit = 0;
        while (partial_BigInt >= divisor) {
            partial_BigInt -= divisor;
            quotient_digit++;
        }

        quotient.number += std::to_string(quotient_digit);
        partial_dividend = partial_BigInt.number;
    }

    if (other.positive == this->positive) {
        quotient.positive = true;
    } else {
        quotient.positive = false;
    }

    quotient.remove_leading_zeroes();
    return quotient;
}

BigInt &BigInt::operator/=(const BigInt &other) {
    *this = *this / other;
    return *this;
}

BigInt BigInt::operator%(const BigInt &other) const {
    BigInt quotient = *this / other;
    BigInt remainder = *this - (other * quotient);

    remainder.positive = true;
    if (remainder.number == "0") {
        return BigInt("0");
    }

    return remainder;
}

BigInt &BigInt::operator%=(const BigInt &other) {
    *this = *this % other;
    return *this;
}

BigInt BigInt::gcd(const BigInt &other) {
    BigInt num1 = *this;
    BigInt num2 = other;

    num1.positive = true;
    num2.positive = true;

    if (num1 == BigInt("0")) {
        return num2;
    }
    if (num2 == BigInt("0")) {
        return num1;
    }

    while (num2 != 0) {
        BigInt remainder = num1 % num2;
        num1 = num2;
        num2 = remainder;
    }
    return num1;
}

// COMPARE OPERATORS
bool BigInt::compare(std::string other_num) const {
    if (number.size() > other_num.size()) return true;
    else if (number.size() < other_num.size()) return false;
    else {
        for (int i = 0; i < number.size(); i++) {
            if (number[i] - '0' > other_num[i] - '0') {
                return true;
            } else if (number[i] - '0' < other_num[i] - '0') {
                return false;
            }
        }
        return true;
    }
}

bool BigInt::operator==(const BigInt &other) const {
    if ((positive && other.positive) || (!positive && !other.positive)) {
        return compare(other.number);
    }
    return false;
}

bool BigInt::operator!=(const BigInt &other) const {
    return !(*this == other);
}

bool BigInt::operator<(const BigInt &other) const {
    if (!positive && other.positive) {
        return true;
    } else if (positive && !other.positive) {
        return false;
    } else if (!positive && !other.positive) {
        // Both numbers are negative, so the logic is inverted
        return compare(other.number) > 0;
    }

    // Both numbers are positive
    if (number.size() < other.number.size()) return true;
    else if (number.size() > other.number.size()) return false;
    else {
        for (int i = 0; i < number.size(); i++) {
            if (number[i] - '0' > other.number[i] - '0') {
                return false;
            } else if (number[i] - '0' < other.number[i] - '0') {
                return true;
            }
        }
        return false;
    }
}

bool BigInt::operator<=(const BigInt &other) const {
    if (*this == other || *this < other) {
        return true;
    }
    return false;
}

bool BigInt::operator>(const BigInt &other) const {
    return !(*this < other);
}

bool BigInt::operator>=(const BigInt &other) const {
    return (*this > other) || (*this == other);
}

BigInt BigInt::operator+(int other) {
    BigInt temp(other);
    return *this + temp;
}

BigInt &BigInt::operator+=(int other) {
    BigInt temp(other);
    *this += temp;
    return *this;
}

BigInt BigInt::operator-(int other) {
    BigInt temp(other);
    return *this - temp;
}

BigInt &BigInt::operator-=(int other) {
    BigInt temp(other);
    *this -= temp;
    return *this;
}

BigInt BigInt::operator*(int other) {
    BigInt temp(other);
    return *this * temp;
}

BigInt &BigInt::operator*=(int other) {
    BigInt temp(other);
    *this *= temp;
    return *this;
}

BigInt BigInt::operator/(int other) {
    BigInt temp(other);
    return *this / temp;
}

BigInt &BigInt::operator/=(int other) {
    BigInt temp(other);
    *this /= temp;
    return *this;
}

BigInt BigInt::operator%(int other) {
    BigInt temp(other);
    return *this % temp;
}

BigInt &BigInt::operator%=(int other) {
    BigInt temp(other);
    *this %= temp;
    return *this;
}

//Std iostream
std::ostream &operator<<(std::ostream &os, const BigInt &num) {
    if (!num.positive && num.number[0] != '0') {
        os << "-";
    }
    os << num.number;
    return os;
}

std::istream &operator>>(std::istream &is, BigInt &num) {
    std::string input;
    for (char c: input) {
        if (c == ' ') throw InvalidInputException();;
    }
    is >> input;
    num = BigInt(input);
    return is;
}

void BigInt::remove_leading_zeroes() {
    size_t nonZeroIndex = number.find_first_not_of('0');
    if (nonZeroIndex != std::string::npos) {
        number = number.substr(nonZeroIndex);
    } else {
        number = "0";
    }
}

std::string BigInt::to_string() const {
    return number;
}

std::string BigInt::module(std::string str) {
    if (str[0] == '-') return str.substr(1);
    else return str;
}

BigInt BigInt::factorial(int n) {
    BigInt res = 1;
    for (int i = 1; i <= n; i++) {
        res *= i;
    }
    return res;
}

// Postfix Increment Operator (num++)
BigInt BigInt::operator++(int) {
    BigInt *temp = this;
    BigInt one("1");
    *this += one;
    return *temp;
}

// Postfix Decrement Operator (num--)//?
BigInt BigInt::operator--(int) {
    BigInt *temp = this;
    BigInt one("1");
    *this -= one;
    return *temp;
}

// Unary minus
BigInt BigInt::operator-() {
    BigInt negated = *this;
    negated.positive = !positive;
    return negated;
}

// Unary plus
BigInt BigInt::operator+() {
    return *this;
}

BigInt &BigInt::operator++() {
    // Prefix increment operator (++num)
    *this += 1;
    return *this;
}

BigInt &BigInt::operator--() {
    // Prefix decrement operator (--num)
    *this -= 1;
    return *this;
}

// Friend function for addition with a BigInt object and an integer
BigInt operator+(const BigInt &a, const int &b) {
    return a + BigInt(b);
}

// Friend function for addition with an integer and a BigInt object
BigInt operator+(const int &a, const BigInt &b) {
    return BigInt(a) + b;
}

BigInt BigInt::pow(int exponent) {
    if (exponent < 0) {
        throw InvalidExponent();
    }

    if (exponent == 0) {
        if (*this == BigInt("0")) {
            throw InvalidExponent();
        } else return BigInt("1");
    }

    BigInt result(this->number);

    for (int i = 1; i < exponent; ++i) {
        result *= (*this);
    }

    if (exponent % 2 == 1 && this->positive == false) {
        result.positive = false;
    } else {
        result.positive = true;
    }
    return result;
}

BigInt BigInt::karatsubaMultiply(const BigInt &X, const BigInt &Y) {
    if (X < 10 && Y < 10) {
        return X * Y;
    }

    int size = std::max(X.to_string().size(), Y.to_string().size());
    size = (size / 2) + (size % 2);

    BigInt multiplier("1");
    for (int i = 0; i < size; ++i) {
        multiplier *= 10;
    }

    BigInt b = X / multiplier;
    BigInt a = X - (b * multiplier);
    BigInt d = Y / multiplier;
    BigInt c = Y - (d * multiplier);

    BigInt u = karatsubaMultiply(a, c);
    BigInt z = karatsubaMultiply(a + b, c + d);
    BigInt v = karatsubaMultiply(b, d);

    return u + ((z - u - v) * multiplier) + (v * (multiplier * multiplier));
}