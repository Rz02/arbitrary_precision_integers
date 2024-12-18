/**
 * @file bigint.hpp
 * @author Zicheng Zhao (zhaoz149@mcmaster.ca)
 * @brief A file that contains the class bigint
 * @version 0.1
 * @date 2024-12-18
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
        // If the number is negative, output the minus sign
        if (num.is_negative)
            out << '-';

        // Iterate through the digits stored in the vector in reversed order
        // from the most significant to the least significant digit
        for (size_t i = num.vec.size(); i > 0; --i)

            // Convert each digit to a character and output it
            out << static_cast<char>('0' + num.vec[i - 1]);

        // Return the stream
        return out;
    }

public:
    /**
     * @brief Checks if two bigint numbers are equal
     *
     * @param rhs The bigint to compare with
     * @return true if both bigint numbers are equal
     * @return false otherwise
     */
    bool operator==(const bigint &rhs) const
    {
        return (is_negative == rhs.is_negative) && (vec == rhs.vec);
    }

    /**
     * @brief Check if two bigint numbers are not equal
     *
     * @param rhs The bigint to compare with
     * @return true if the bigint numbers are not equal
     * @return false otherwise
     */
    bool operator!=(const bigint &rhs) const
    {
        return !(*this == rhs);
    }

    /**
     * @brief Compares if the current bigint is less than the given bigint
     *
     * @param rhs The bigint to compare with
     * @return true if the current bigint is less than the given bigint
     * @return false otherwise
     */
    bool operator<(const bigint &rhs) const
    {
        // Check if the signs are different
        // A negative number is always less than a positive one
        if (is_negative != rhs.is_negative)
            return is_negative;

        // Compare the size of the vectors
        // Larger size implies a larger absolute value
        if (vec.size() != rhs.vec.size())
            return is_negative ? (vec.size() > rhs.vec.size()) : (vec.size() < rhs.vec.size());

        // Compare digit-by-digit starting from the most significant digit
        for (size_t i = vec.size(); i > 0; --i)
        {
            if (vec[i - 1] != rhs.vec[i - 1])
                return is_negative ? (vec[i - 1] > rhs.vec[i - 1]) : (vec[i - 1] < rhs.vec[i - 1]);
        }

        // If all digits are equal, the numbers are equal
        return false;
    }

    /**
     * @brief Compares if the current bigint is less than or equal to the given bigint
     *
     * @param rhs The bigint to compare with
     * @return true if the current bigint is less than or equal to the given bigint
     * @return false otherwise
     */
    bool operator<=(const bigint &rhs) const
    {
        return (*this < rhs) || (*this == rhs);
    }

    /**
     * @brief Compares if the current bigint is greater than the given bigint
     *
     * @param rhs The bigint to compare with
     * @return true if the current bigint is greater than the given bigint
     * @return false otherwise
     */
    bool operator>(const bigint &rhs) const
    {
        return !(*this <= rhs);
    }

    /**
     * @brief Compares if the current bigint is greater than or equal to the given bigint
     *
     * @param rhs The bigint to compare with
     * @return true if the current bigint is greater than or equal to the given bigint
     * @return false otherwise
     */
    bool operator>=(const bigint &rhs) const
    {
        return !(*this < rhs);
    }

    /**
     * @brief Adds two bigint numbers
     *
     * @param rhs The bigint to add to the current bigint
     * @return bigint A new one representing the sum
     */
    bigint operator+(const bigint &rhs) const
    {
        // If the signs are the same, add the absolute values
        if (is_negative == rhs.is_negative)
            return bigint(is_negative, add_vec(vec, rhs.vec));

        // If the signs differ, perform subtraction of the absolute values
        else if (abs_compare(vec, rhs.vec) >= 0)
            return bigint(is_negative, subtract_vec(vec, rhs.vec));
        else
            return bigint(rhs.is_negative, subtract_vec(rhs.vec, vec));
    }

    /**
     * @brief Subtracts the given bigint from the current bigint
     *
     * @param rhs The bigint to subtract
     * @return bigint A new one representing the difference
     */
    bigint operator-(const bigint &rhs) const
    {
        // If the signs differ, perform addition of the absolute values
        if (is_negative != rhs.is_negative)
            return bigint(is_negative, add_vec(vec, rhs.vec));

        // If the signs are the same, subtract the absolute values
        else if (abs_compare(vec, rhs.vec) >= 0)
            return bigint(is_negative, subtract_vec(vec, rhs.vec));
        else
            return bigint(!is_negative, subtract_vec(rhs.vec, vec));
    }

    /**
     * @brief Multiplies two bigint numbers
     *
     * @param rhs The bigint to multiply with the current bigint
     * @return bigint A new one representing the product
     */
    bigint operator*(const bigint &rhs) const
    {
        // Initialize a vector to hold the product
        // It's properly sized for maximum possible digits
        std::vector<uint8_t> product(vec.size() + rhs.vec.size(), 0);

        // Perform digit-by-digit multiplication
        for (size_t i = 0; i < vec.size(); ++i)
        {
            int carry = 0;
            for (size_t j = 0; j < rhs.vec.size() || carry; ++j)
            {
                int64_t current = product[i + j] + vec[i] * (j < rhs.vec.size() ? rhs.vec[j] : 0) + carry;
                product[i + j] = static_cast<uint8_t>(current % 10);
                carry = static_cast<uint8_t>(current / 10);
            }
        }

        // Determine the sign of the product and return the result
        return bigint(is_negative != rhs.is_negative, product);
    }

    /**
     * @brief Divides the current bigint by the given bigint
     *
     * @param rhs The bigint divisor
     * @return bigint A new one representing the quotient
     */
    bigint operator/(const bigint &rhs) const
    {
        // Handle the division by zero
        if (rhs == 0)
            throw std::invalid_argument("Division by zero");

        // Make both dividend and divisor positive for calculation
        bigint dividend(*this);
        bigint divisor(rhs);
        dividend.is_negative = false;
        divisor.is_negative = false;

        // If the divisor is greater than the dividend, the result is zero
        if (abs_compare(dividend.vec, divisor.vec) < 0)
            return bigint(0);

        // Initialize variables for the quotient and the current value being divided
        std::vector<uint8_t> quotient;
        bigint current(0);

        // Perform long division
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

        // Reverse the quotient vector and set the correct sign
        std::reverse(quotient.begin(), quotient.end());
        return bigint(is_negative != rhs.is_negative, quotient);
    }

    /**
     * @brief Computes the modulus of the current bigint divided by the given bigint
     *
     * @param rhs The bigint divisor
     * @return bigint A new one representing the remainder
     */
    bigint operator%(const bigint &rhs) const
    {
        // Handle the modulus by zero
        if (rhs == 0)
            throw std::invalid_argument("Modulus by zero");

        // Make both dividend and divisor positive for calculation
        bigint dividend(*this);
        bigint divisor(rhs);
        dividend.is_negative = false;
        divisor.is_negative = false;

        // If the dividend is smaller than the divisor, return the dividend
        if (dividend < divisor)
            return *this;

        // Perform modulus operation
        bigint current(0);
        for (size_t i = dividend.vec.size(); i > 0; --i)
        {
            current.vec.insert(current.vec.begin(), dividend.vec[i - 1]);
            current.trim();

            while (current >= divisor)
                current -= divisor;
        }

        // Set the correct sign for the remainder
        current.is_negative = is_negative;
        if (current == 0)
            current.is_negative = false;

        return current;
    }

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
     * @brief Converts the bigint to its string representation in the specified base.
     *
     * This function converts the bigint into a string representation for the given base.
     * Supported bases are between 2 and 36 (inclusive). The digits are represented as
     * '0'-'9' for values 0-9 and 'A'-'Z' for values 10-35.
     *
     * @param base The base for the string representation (must be between 2 and 36).
     * @return std::string The string representation of the bigint in the specified base.
     */
    std::string to_string(uint64_t base) const
    {
        if (base < 2 || base > 36)
        {
            throw std::invalid_argument("Base must be between 2 and 36.");
        }

        if (vec.empty() || (vec.size() == 1 && vec[0] == 0))
        {
            // Return "0" if the number is zero
            return "0";
        }

        // Create a copy of the current bigint to work on
        bigint temp = *this;
        // Ignore sign for digit extraction
        temp.is_negative = false;

        std::string result;

        // Characters used for digits in bases up to 36
        // Pointer seems to work here too:
        // const char *digits = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        // However, for avoiding using pointer, we'll use reference instead:
        constexpr const char(&digits)[37] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

        // Convert the number to the desired base
        // Until the number becomes zero
        while (!(temp.vec.size() == 1 && temp.vec[0] == 0))
        {
            bigint remainder(0);
            temp = temp.divide_by_base(base, remainder);

            // Remainder will always be less than the base
            int rem_value = remainder.vec[0];
            result.push_back(digits[rem_value]);
        }

        // Reverse the result string as digits were obtained in reverse order
        std::reverse(result.begin(), result.end());

        // Add '-' sign if the original number was negative
        if (is_negative)
        {
            result = "-" + result;
        }

        return result;
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
            // Convert the number to a vector of digits in reverse order
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
            //
            // # https://stackoverflow.com/questions/12927087/what-does-string-0-do-string-is-a-char
            //
            // This subtracts from the character to which string is pointing
            // the ASCII code of the character '0'.
            // So, '0' - '0' gives you 0 and so on and '9' - '0' gives you 9.
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
            // ASCII
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

    /**
     * @brief Divides the bigint by a given base and calculates the remainder.
     *
     * This function performs division of the current bigint object by the specified base
     * and returns the resulting quotient as a new bigint. The remainder of the division
     * is stored in the provided `remainder` parameter as a bigint. This method works
     * by processing the bigint digit-by-digit, handling carry values to simulate manual division.
     *
     * @param base The base to divide the bigint by (must be greater than 1).
     * @param remainder A reference to a bigint object where the remainder will be stored.
     * @return bigint The quotient of the division as a bigint.
     */
    bigint divide_by_base(uint64_t base, bigint &remainder) const
    {
        bigint result;
        result.vec.resize(vec.size());

        uint64_t carry = 0;

        for (size_t i = vec.size(); i > 0; --i)
        {
            carry = carry * 10 + vec[i - 1];
            result.vec[i - 1] = static_cast<uint8_t>(carry / base);
            carry %= base;
        }

        remainder = bigint(static_cast<int>(carry));
        result.trim();
        return result;
    }
};
