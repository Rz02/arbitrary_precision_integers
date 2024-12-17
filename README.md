# Arbitrary Precision Integers

## Overview
The **bigint** class is designed for **arbitrary-precision integer arithmetic**. It allows for manipulating large integers, beyond the typical limits of standard integer types in C++, by storing the digits of the number in **a vector of uint8_t**. This class supports basic arithmetic operations like addition, subtraction, multiplication, division, modulus, and comparison operators, making it suitable for applications requiring high-precision numbers.

## Features
Arbitrary Precision: 
- Supports integers of any size, limited only by system memory.

Signed Arithmetic: 
- Handles both positive and negative integers.

Basic Operations: 
- Supports addition, subtraction, multiplication, division, modulus, and negation.
- Supports increment and decrement operators in both pre and post-forms.

Comparison: 
- Supports comparisons (`==, !=, <, <=, >, >=`).

Input/Output: 
- Can be initialized from strings or integers, and output to streams.

## Usage
- Note:

    Include the header file `#include "bigint.hpp"` in the program to use the `bigint` class.
- Constructors:

    1. **Default**: Create a `bigint` with a value of `0`:
       - `bigint num;`
    2. **Integer**: Initialize a `bigint` from a 64-bit integer:
       - `bigint num(123456789);`
    3. **String**: Initialize a `bigint` from a string of digits. Supports both positive and negative numbers:
       - `bigint num("-123456789");`
    4. **String & Base**: Initialize a `bigint` from a string and a base by converting the string to an arbitrary-precision integer in that base:
       - `bigint num("FF", 16);`
- Operators:

    - Arithmetic:
        * `+`: add two `bigint` objects
        * `-`: subtract one `bigint` from another
        * `*`: multiply two `bigint` objects
        * `/`: divide one `bigint` from another
          - uses long division to compute the quotient digit by digit
          - throws `std::invalid_argument` for division by zero
        * `%`: return the remainder of one `bigint` divided by another
          - uses long division to compute the remainder
          - maintains the sign of the dividend in the result
          - throws `std::invalid_argument` for modulus by zero

    - Compound Assignment:
        * `+=, -=, *=, /=, %=`: Perform arithmetic operations and update the current object.
        * `++, --`: Increment and decrement operators (both pre- and post- forms)

    - Unary:
        * `-`: Negates a `bigint` value

    - Comparison:
        * `==, !=, <, <=, >, >=`: Compare two `bigint` objects

    - Insertion:
        * `<<` outputs the value of the `bigint` object to the provided stream

## Friend Function
- Insertion:
  * `friend std::ostream &operator<<(std::ostream &out, const bigint &num)`
  * ```
    // E.g.
    bigint num(12345);
    std::cout << num; // Output: 12345
    ```

## Member Functions
### Public:
1. Comparison:
    - `bool operator==(const bigint &rhs) const`
    - `bool operator!=(const bigint &rhs) const`
    - `bool operator<(const bigint &rhs) const`
    - `bool operator<=(const bigint &rhs) const`
    - `bool operator>(const bigint &rhs) const`
    - `bool operator>=(const bigint &rhs) const`
    - ```
      // E.g.
      bigint a(123);
      bigint b(456);
      std::cout << (a < b); // Output: 1 (true)
      ```

2. Arithmetic:
    - `bigint operator+(const bigint &rhs) const`
    - `bigint operator-(const bigint &rhs) const`
    - `bigint operator*(const bigint &rhs) const`
    - `bigint operator/(const bigint &rhs) const`
    - `bigint operator%(const bigint &rhs) const`
    - ```
      // E.g.
      bigint a(12345);
      bigint b(-6789);
      std::cout << a + b; // Output: 5556

      bigint c(12345);
      bigint d(123);
      std::cout << c / d; // Output: 100
      ```

3. Compound Assignment:
    - `bigint &operator+=(const bigint &rhs)`
    - `bigint &operator-=(const bigint &rhs)`
    - `bigint &operator*=(const bigint &rhs)`
    - `bigint &operator/=(const bigint &rhs)`
    - `bigint &operator%=(const bigint &rhs)`
    - ```
      // E.g.
      bigint num(10);
      num += bigint(5);
      std::cout << num; // Output: 15
      ```

4. Unary: `-`
    - Negates the `bigint` value
    - ```
      // E.g.
      bigint num(123);
      std::cout << -num; // Output: -123
      ```

5. Pre/Post Increment/Decrement:
    - `bigint &operator++()`: Pre-increment
    - `bigint operator++(int)`: Post-increment
    - `bigint &operator--()`: Pre-decrement
    - `bigint operator--(int)`: Post-decrement
    - ```
      // E.g.
      bigint num(10);
      ++num;
      std::cout << num; //Output: 11
      ```

6. Default: `bigint()`
    - Create a `bigint` initialized to `0`
    - ```
      // E.g.
      bigint num;
      std::cout << num; // Output: 0
      ```

7. Integer: `bigint(int64_t num)`
    - Initialize a `bigint` from an integer
    - ```
      // E.g.
      bigint num(12345);
      std::cout << num; // Output: 12345
      ```

8. String: `bigint(const std::string &str)`
    - Convert a string representation into a `bigint`
    - Supports both positive and negative values
    - Throws `std::invalid_argument` for invalid input
    - ```
      // E.g.
      bigint num("-987654321");
      std::cout << num; // Output: -987654321
      ```

9. String with Base: `bigint(const std::string &str, int base)`
    - Constructs a `bigint` from a string in the specified base (2 to 36)
    - Throws `std::invalid_argument` for invalid bases or characters
    - ```
      // E.g.
      bigint num("FF", 16);
      std:cout << num; // Output: 255
      ```

### Private:
1. `trim()`:
    - Remove leading zeros from the internal representation.
2. `add_vec(const std::vector<uint8_t> &a, const std::vector<uint8_t> &b)`:
    - Add two vectors.
3. `subtract_vec(const std::vector<uint8_t> &a, const std::vectro<uint8_t> &b)`:
    - Subtract one vector from another.
4. `abs_compare(const std::vectro<uint8_t> &a, const std::vector<uint8_t> &b)`:
    - Compare two vectors.
