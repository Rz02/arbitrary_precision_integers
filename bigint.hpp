#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>

class bigint
{
    // << overloading
    friend std::ostream &operator<<(std::ostream &out, const bigint &num)
    {
        if (num.is_negative)
            out << '-';
        for (size_t i = num.vec.size(); i > 0; --i)
            out << static_cast<char>('0' + num.vec[i - 1]);
        return out;
    }

    // Comparison overloading

    // == & !=
    friend bool operator==(const bigint &lhs, const bigint &rhs)
    {
        return (lhs.is_negative == rhs.is_negative) && (lhs.vec == rhs.vec);
    }

    friend bool operator!=(const bigint &lhs, const bigint &rhs)
    {
        return !(lhs == rhs);
    }

    // <, <= & >, >=
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

    friend bool operator<=(const bigint &lhs, const bigint &rhs)
    {
        return (lhs < rhs) || (lhs == rhs);
    }

    friend bool operator>(const bigint &lhs, const bigint &rhs)
    {
        return !(lhs <= rhs);
    }

    friend bool operator>=(const bigint &lhs, const bigint &rhs)
    {
        return !(lhs < rhs);
    }

    // +, -, *
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

    // /
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

    // %
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
    // +=, -=, *=
    bigint &operator+=(const bigint &rhs)
    {
        *this = *this + rhs;
        return *this;
    }

    bigint &operator-=(const bigint &rhs)
    {
        *this = *this - rhs;
        return *this;
    }

    bigint &operator*=(const bigint &rhs)
    {
        *this = *this * rhs;
        return *this;
    }

    // /=
    bigint &operator/=(const bigint &rhs)
    {
        *this = *this / rhs;
        return *this;
    }

    // %=
    bigint &operator%=(const bigint &rhs)
    {
        *this = *this % rhs;
        return *this;
    }

    // Unary -
    bigint operator-() const
    {
        return bigint(!is_negative, vec);
    }

    // Pre-/Post- increment/decrement of ++ and --
    bigint &operator++()
    {
        *this = *this + 1;
        return *this;
    }

    bigint operator++(int)
    {
        bigint temp = *this;
        *this = *this + 1;
        return temp;
    }

    bigint &operator--()
    {
        *this = *this - 1;
        return *this;
    }

    bigint operator--(int)
    {
        bigint temp = *this;
        *this = *this - 1;
        return temp;
    }

    // Constructor without arguments
    // Create the integer 0
    bigint() : vec({0}) {}

    // Constructor with one argument
    // Take a int64_t
    // Convert it to an arbitrary-precision integer
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

    // Constructor with one argument
    // Take a string of digits
    // Convert it to an arbitrary-precision integer
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

private:
    bool is_negative = false;
    std::vector<uint8_t> vec = std::vector<uint8_t>();
    bigint(bool negative, const std::vector<uint8_t> &vector) : is_negative(negative), vec(vector) { trim(); }

    // Trim leading zeros
    void trim()
    {
        while (vec.size() > 1 && vec.back() == 0)
            vec.pop_back();
        if (vec.size() == 1 && vec[0] == 0)
            is_negative = false;
    }

    // Add two vectors
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

    // Subtract two vectors
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

    // Compare absolute values of two vectors
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
