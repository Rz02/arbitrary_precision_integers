/**
 * @file bigint.hpp
 * @author Zicheng Zhao (zhaoz149@mcmaster.ca)
 * @brief A file that contains the class bigint
 * @version 0.1
 * @date 2024-12-04
 *
 * @copyright Copyright (c) 2024
 *
 */
#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>

/**
 * @brief A class for arbitrary-precision integers
 *
 */
class bigint
{
    /**
     * @brief I/O operator for insertion overloading
     *
     * @param out an instance of std::ostream
     * @param num the bigint object whose value will be output
     * @return std::ostream& A reference to the same output stream that's passed as an argument
     */
    friend std::ostream &operator<<(std::ostream &out, const bigint &num)
    {
        if (num.is_negative)
            out << '-';
        for (size_t i = num.vec.size(); i > 0; --i)
            out << static_cast<char>('0' + num.vec[i - 1]);
        return out;
    }

    /**
     * @brief comparison operator == overloading
     *
     * @param lhs left hand side
     * @param rhs right hand side
     * @return true If meet the criteria
     * @return false If not meet the criteria
     */
    friend bool operator==(const bigint &lhs, const bigint &rhs)
    {
        return (lhs.is_negative == rhs.is_negative) && (lhs.vec == rhs.vec);
    }

    /**
     * @brief comparison operator != overloading
     *
     * @param lhs left hand side
     * @param rhs right hand side
     * @return true If meet the criteria
     * @return false If not meet the criteria
     */
    friend bool operator!=(const bigint &lhs, const bigint &rhs)
    {
        return !(lhs == rhs);
    }

    /**
     * @brief comparison operator < overloading
     *
     * @param lhs left hand side
     * @param rhs right hand side
     * @return true If meet the criteria
     * @return false If not meet the criteria
     */
    friend bool operator<(const bigint &lhs, const bigint &rhs)
    {
        if (lhs.is_negative != rhs.is_negative)
            return lhs.is_negative;
        if (lhs.vec.size() != rhs.vec.size())
            return lhs.is_negative ? (lhs.vec.size() > rhs.vec.size()) : (lhs.vec.size() < rhs.vec.size());
        for (size_t i = lhs.vec.size(); i > 0; --i)
        {
            if (lhs.vec[i - 1] != rhs.vec[i - 1])
                return lhs.is_negative ? (lhs.vec[i - 1] > rhs.vec[i - 1]) : (lhs.vec[i - 1] < rhs.vec[i - 1]);
        }
        return false;
    }

    /**
     * @brief comparison operator <= overloading
     *
     * @param lhs left hand side
     * @param rhs right hand side
     * @return true If meet the criteria
     * @return false If not meet the criteria
     */
    friend bool operator<=(const bigint &lhs, const bigint &rhs)
    {
        return (lhs < rhs) || (lhs == rhs);
    }

    /**
     * @brief comparison operator > overloading
     *
     * @param lhs left hand side
     * @param rhs right hand side
     * @return true If meet the criteria
     * @return false If not meet the criteria
     */
    friend bool operator>(const bigint &lhs, const bigint &rhs)
    {
        return !(lhs <= rhs);
    }

    /**
     * @brief comparison operator >= overloading
     *
     * @param lhs left hand side
     * @param rhs right hand side
     * @return true If meet the criteria
     * @return false If not meet the criteria
     */
    friend bool operator>=(const bigint &lhs, const bigint &rhs)
    {
        return !(lhs < rhs);
    }

    /**
     * @brief arithmetic operator + overloading
     *
     * @param lhs left hand side
     * @param rhs right hand side
     * @return bigint The sum of the two bigint
     */
    friend bigint operator+(const bigint &lhs, const bigint &rhs)
    {
        if (lhs.is_negative == rhs.is_negative)
        {
            return bigint(lhs.is_negative, add_vec(lhs.vec, rhs.vec));
        }
        else if (abs_compare(lhs.vec, rhs.vec) >= 0)
        {
            return bigint(lhs.is_negative, subtract_vec(lhs.vec, rhs.vec));
        }
        else
        {
            return bigint(rhs.is_negative, subtract_vec(rhs.vec, lhs.vec));
        }
    }

    /**
     * @brief arithmetic operator - overloading
     *
     * @param lhs left hand side
     * @param rhs right hand side
     * @return bigint The first bigint minus the second bigint
     */
    friend bigint operator-(const bigint &lhs, const bigint &rhs)
    {
        if (lhs.is_negative != rhs.is_negative)
        {
            return bigint(lhs.is_negative, add_vec(lhs.vec, rhs.vec));
        }
        else if (abs_compare(lhs.vec, rhs.vec) >= 0)
        {
            return bigint(lhs.is_negative, subtract_vec(lhs.vec, rhs.vec));
        }
        else
        {
            return bigint(!lhs.is_negative, subtract_vec(rhs.vec, lhs.vec));
        }
    }

    /**
     * @brief arithmetic operator * overloading
     *
     * @param lhs left hand side
     * @param rhs right hand side
     * @return bigint The multiplication of the two bigint
     */
    friend bigint operator*(const bigint &lhs, const bigint &rhs)
    {
        std::vector<uint8_t> product(lhs.vec.size() + rhs.vec.size(), 0);
        for (size_t i = 0; i < lhs.vec.size(); ++i)
        {
            int carry = 0;
            for (size_t j = 0; j < rhs.vec.size() || carry; ++j)
            {
                int64_t current = product[i + j] + lhs.vec[i] * (j < rhs.vec.size() ? rhs.vec[j] : 0) + carry;
                product[i + j] = static_cast<uint8_t>(current % 10);
                carry = static_cast<uint8_t>(current / 10);
            }
        }
        return bigint(lhs.is_negative != rhs.is_negative, product);
    }

    /**
     * @brief arithmetic operator / overloading
     *
     * @param lhs left hand side
     * @param rhs right hand side
     * @return bigint The first bigint divided by the second bigint
     */
    friend bigint operator/(const bigint &lhs, const bigint &rhs)
    {
        if (rhs == 0)
            throw std::invalid_argument("Division by zero");

        bigint dividend(lhs);
        bigint divisor(rhs);
        dividend.is_negative = false;
        divisor.is_negative = false;

        if (abs_compare(dividend.vec, divisor.vec) < 0)
            return bigint(0);

        std::vector<uint8_t> quotient;
        bigint current(0);

        for (size_t i = dividend.vec.size(); i > 0; --i)
        {
            current.vec.insert(current.vec.begin(), dividend.vec[i - 1]);
            current.trim();

            uint8_t count = 0;
            while (current >= divisor)
            {
                current -= divisor;
                ++count;
            }
            quotient.push_back(count);
        }
        std::reverse(quotient.begin(), quotient.end());
        return bigint(lhs.is_negative != rhs.is_negative, quotient);
    }

    /**
     * @brief arithmetic operator % overloading
     *
     * @param lhs left hand side
     * @param rhs right hand side
     * @return bigint The remainder of the first bigint divided by the second bigint
     */
    friend bigint operator%(const bigint &lhs, const bigint &rhs)
    {
        if (rhs == 0)
            throw std::invalid_argument("Modulus by zero");

        bigint dividend(lhs);
        bigint divisor(rhs);

        dividend.is_negative = false;
        divisor.is_negative = false;

        if (dividend < divisor)
            return lhs;

        bigint current(0);
        for (size_t i = dividend.vec.size(); i > 0; --i)
        {
            current.vec.insert(current.vec.begin(), dividend.vec[i - 1]);
            current.trim();

            while (current >= divisor)
                current -= divisor;
        }

        current.is_negative = lhs.is_negative;
        if (current == 0)
            current.is_negative = false;

        return current;
    }

public:
    /**
     * @brief compound assignment += overloading
     *
     * @param rhs right hand side
     * @return bigint& A reference to the current object after the operation
     */
    bigint &operator+=(const bigint &rhs)
    {
        *this = *this + rhs;
        return *this;
    }

    /**
     * @brief compound assignment -= overloading
     *
     * @param rhs right hand side
     * @return bigint& A reference to the current object after the operation
     */
    bigint &operator-=(const bigint &rhs)
    {
        *this = *this - rhs;
        return *this;
    }

    /**
     * @brief compound assignment *= overloading
     *
     * @param rhs right hand side
     * @return bigint& A reference to the current object after the operation
     */
    bigint &operator*=(const bigint &rhs)
    {
        *this = *this * rhs;
        return *this;
    }

    /**
     * @brief compound assignment /= overloading
     *
     * @param rhs right hand side
     * @return bigint& A reference to the current object after the operation
     */
    bigint &operator/=(const bigint &rhs)
    {
        *this = *this / rhs;
        return *this;
    }

    /**
     * @brief compound assignment %= overloading
     *
     * @param rhs right hand side
     * @return bigint& A reference to the current object after the operation
     */
    bigint &operator%=(const bigint &rhs)
    {
        *this = *this % rhs;
        return *this;
    }

    /**
     * @brief Unary operator - overloading
     *
     * @return bigint A negated bigint value
     */
    bigint operator-() const
    {
        return bigint(!is_negative, vec);
    }

    /**
     * @brief Pre-increment operator overload
     *
     * @return bigint& Reference to the current object after the operation
     */
    bigint &operator++()
    {
        *this = *this + 1;
        return *this;
    }

    /**
     * @brief Post-increment operator overload
     *
     * @return bigint A copy of the object as it was before the operation
     */
    bigint operator++(int)
    {
        bigint temp = *this;
        *this = *this + 1;
        return temp;
    }

    /**
     * @brief Pre-decrement operator overload
     *
     * @return bigint& Reference to the current object after the operation
     */
    bigint &operator--()
    {
        *this = *this - 1;
        return *this;
    }

    /**
     * @brief Post-decrement operator overload
     *
     * @return bigint A copy of the object as it was before the operation
     */
    bigint operator--(int)
    {
        bigint temp = *this;
        *this = *this - 1;
        return temp;
    }

    /**
     * @brief Construct a new bigint object with an initial value of 0.
     *
     */
    bigint() : vec({0}) {}

    /**
     * @brief Construct a new bigint object from a signed 64-bit integer.
     *
     * Initializes the bigint object to represent the provided integer,
     * supporting both positive and negative values.
     *
     * @param num The signed 64-bit integer to initialize the bigint object with.
     */
    bigint(int64_t num)
    {
        // If num is negative
        if (num < 0)
        {
            is_negative = true;
            num = -num;
        }
        else
        {
            is_negative = false;
        }

        // If num is zero
        if (num == 0)
        {
            vec.push_back(0);
        }

        // If num is positive
        while (num > 0)
        {
            vec.push_back(static_cast<uint8_t>(num % 10));
            num /= 10;
        }
    }

    /**
     * @brief Construct a new bigint object from a string representation.
     *
     * Initializes the bigint object to represent the number provided as a string.
     * Supports both positive and negative numbers.
     *
     * @param str The string representing the number. It can optionally start with
     * a '-' for negative numbers. The rest of the string must only contain digits.
     */
    bigint(const std::string &str)
    {
        // If str is empty
        if (str.empty())
            throw std::invalid_argument("String cannot be empty!");

        // If str represents negative number
        size_t index = 0;
        if (str[0] == '-')
        {
            is_negative = true;
            index = 1;
        }
        else
        {
            is_negative = false;
        }

        // If str contains invalid character
        for (size_t i = str.size(); i > index; --i)
        {
            if (!std::isdigit(str[i - 1]))
                throw std::invalid_argument("Invalid character in string!");
            vec.push_back(static_cast<uint8_t>(str[i - 1] - '0'));
        }

        // Trim leading zero and avoid Negative zero
        trim();
    }

    /**
     * @brief Construct a new bigint object from a string representation in a given base.
     *
     * This constructor takes a string that represents a large integer in a specific base
     * (from 2 to 36) and converts it into a `bigint` object.
     * The string may optionally start with a minus sign ('-') to indicate a negative number.
     * The digits in the string can be any combination of digits (0-9) and letters (A-Z or a-z),
     * which represent values in the specified base.
     *
     * @param str The string representing the number to be converted to a bigint.
     * @param base The base in which the number is represented (must be between 2 and 36).
     */
    bigint(const std::string &str, int base)
    {
        if (base < 2 || base > 36)
        {
            throw std::invalid_argument("Base must be between 2 and 36.");
        }

        // Initialize as not negative
        is_negative = false;

        // Check if the number is negative
        size_t start = 0;
        if (str[0] == '-')
        {
            is_negative = true;
            // Skip the negative sign for processing
            start = 1;
        }

        // Keep the status of is_negative in Boolean for tracking
        // Since the for loop later change the is_negative to default again
        // The bool of is_negative affiliates to bigint.
        // It isn't correlated with the is_negative temporary result we obtain here.
        bool negation = static_cast<bool>(is_negative);

        // Initialize the bigint to 0
        *this = bigint(0);

        // Convert the string to the arbitrary-precision integer
        for (size_t i = start; i < str.size(); ++i)
        {
            char c = str[i];
            int digit = 0;

            // Determine the digit value based on the character
            if ('0' <= c && c <= '9')
            {
                digit = c - '0';
            }
            else if ('A' <= c && c <= 'Z')
            {
                digit = c - 'A' + 10;
            }
            else if ('a' <= c && c <= 'z')
            {
                digit = c - 'a' + 10;
            }
            else
            {
                throw std::invalid_argument("Invalid character in string for the given base.");
            }

            if (digit >= base)
            {
                throw std::invalid_argument("Invalid character in string for the given base.");
            }

            *this *= base;
            *this += digit;
        }

        // Apply the negation if the number was negative
        if (negation)
        {
            *this = -*this;
        }
    }

private:
    /**
     * @brief Indicates whether the bigint object represents a negative number.
     *
     * Default is false, meaning the number is positive.
     *
     */
    bool is_negative = false;

    /**
     * @brief Stores the digits of the bigint object in reverse order.
     *
     * Each element represents a single digit (0–9) of the number, with the
     * least significant digit at index 0. Default is an empty vector.
     *
     */
    std::vector<uint8_t> vec = std::vector<uint8_t>();

    /**
     * @brief Construct a new bigint object from a sign indicator and a vector of digits.
     *
     * This constructor initializes the bigint object directly with the provided sign
     * and digit vector.
     *
     * @param negative Indicates if the number is negative (true for negative).
     * @param vector The vector containing the digits of the number in reverse order.
     */
    bigint(bool negative, const std::vector<uint8_t> &vector) : is_negative(negative), vec(vector) { trim(); }

    /**
     * @brief Removes leading zeros from the bigint representation.
     *
     * This method ensures the internal vector `vec` does not contain unnecessary
     * leading zeros. If the number becomes zero, it also resets the sign
     * (`is_negative`) to false.
     *
     */
    void trim()
    {
        while (vec.size() > 1 && vec.back() == 0)
            vec.pop_back();
        if (vec.size() == 1 && vec[0] == 0)
            is_negative = false;
    }

    /**
     * @brief Adds two vectors representing reversed-digit numbers.
     *
     * Performs addition of two numbers stored as vectors of digits in reverse order,
     * handling carry propagation.
     *
     * @param a The first number as a vector of digits in reverse order.
     * @param b The second number as a vector of digits in reverse order.
     * @return std::vector<uint8_t> The result of the addition as a vector of digits in reverse order.
     */
    static std::vector<uint8_t> add_vec(const std::vector<uint8_t> &a, const std::vector<uint8_t> &b)
    {
        std::vector<uint8_t> result = std::vector<uint8_t>();
        size_t max_size = std::max(a.size(), b.size());
        int carry = 0;

        for (size_t i = 0; i < max_size || carry; ++i)
        {
            int sum = carry;
            if (i < a.size())
                sum += a[i];
            if (i < b.size())
                sum += b[i];
            result.push_back(static_cast<uint8_t>(sum % 10));
            carry = sum / 10;
        }
        return result;
    }

    /**
     * @brief Subtracts one vector from another, representing reversed-digit numbers.
     *
     * Performs subtraction of `b` from `a`, assuming `a` represents a number greater
     * than or equal to `b`. Both inputs are stored as vectors of digits in reverse order.
     *
     * @param a The minuend as a vector of digits in reverse order.
     * @param b The subtrahend as a vector of digits in reverse order.
     * @return std::vector<uint8_t> The result of the subtraction as a vector of digits in reverse order.
     */
    static std::vector<uint8_t> subtract_vec(const std::vector<uint8_t> &a, const std::vector<uint8_t> &b)
    {
        std::vector<uint8_t> result = std::vector<uint8_t>();
        int borrow = 0;

        for (size_t i = 0; i < a.size(); ++i)
        {
            int diff = a[i] - borrow;
            if (i < b.size())
                diff -= b[i];
            if (diff < 0)
            {
                diff += 10;
                borrow = 1;
            }
            else
            {
                borrow = 0;
            }
            result.push_back(static_cast<uint8_t>(diff));
        }

        while (result.size() > 1 && result.back() == 0)
            result.pop_back();

        return result;
    }

    /**
     * @brief Compares the absolute values of two numbers represented as digit vectors.
     *
     * Determines whether the absolute value of `a` is greater than, less than, or
     * equal to the absolute value of `b`. Both inputs are stored as vectors of digits
     * in reverse order.
     *
     * @param a The first number as a vector of digits in reverse order.
     * @param b The second number as a vector of digits in reverse order.
     * @return int8_t Returns 1 if |a| > |b|, -1 if |a| < |b|, and 0 if |a| == |b|.
     */
    static int8_t abs_compare(const std::vector<uint8_t> &a, const std::vector<uint8_t> &b)
    {
        if (a.size() != b.size())
            return a.size() > b.size() ? 1 : -1;
        for (size_t i = a.size(); i > 0; --i)
        {
            if (a[i - 1] != b[i - 1])
                return a[i - 1] > b[i - 1] ? 1 : -1;
        }
        return 0;
    }
};
