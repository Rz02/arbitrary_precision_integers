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
- Constructors (*Samples are given later*):

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
          ```
          bigint a(12345);
          bigint b(-6789);
          std::cout << a + b; // Output: 5556
          ```
        * `-`: subtract one `bigint` from another
          ```
          bigint a(12345);
          bigint b(6789);
          std:cout << a - b; // Output: 5556
          ```
        * `*`: multiply two `bigint` objects
          ```
          bigint a(123);
          bigint b(-456);
          std::cout << a * b // Output: -56088
          ```
        * `/`: divide one `bigint` from another
          - uses long division to compute the quotient digit by digit
          - throws `std::invalid_argument` for division by zero
          ```
          bigint a(12345);
          bigint b(123);
          std::cout << a / b; // Output: 100
          ```
        * `%`: return the remainder of one `bigint` divided by another
          - uses long division to compute the remainder
          - maintains the sign of the dividend in the result
          - throws `std::invalid_argument` for modulus by zero
          ```
          bigint a(12345);
          bigint b(123);
          std::cout << a % b; // Output: 45
          ```
    - Compound Assignment (*Samples are given later*):
        * `+=, -=, *=, /=, %=`: Perform arithmetic operations and update the current object.
    - Unary (*Samples are given later*):
        * `-`: Negates a `bigint` value
        * `++, --`: Increment and decrement operators (both pre- and post- forms)
    - Comparison:
        * `==, !=, <, <=, >, >=`: Compare two `bigint` objects
          ```
          bigint a(123);
          bigint b(456);
          std::cout << (a < b); // Output: 1 (true)
          ```
    - `<<` outputs the value of the `bigint` object to the provided stream
      ```
      bigint num(12345);
      std::cout << num; // Output: 12345
      ```
## Member Functions
### Public:
1. Default: `bigint()`
    - Create a `bigint` initialized to `0`
    - ```
      bigint num;
      std::cout << num; // Output: 0
      ```
2. Integer: `bigint(int64_t num)`
    - Initialize a `bigint` from an integer
    - ```
      bigint num(12345);
      std::cout << num; // Output: 12345
      ```
3. String: `bigint(const std::string &str)`
    - Convert a string representation into a `bigint`
    - Supports both positive and negative values
    - Throws `std::invalid_argument` for invalid input
    - ```
      bigint num("-987654321");
      std::cout << num; // Output: -987654321
      ```
4. String with Base: `bigint(const std::string &str, int base)`
    - Constructs a `bigint` from a string in the specified base (2 to 36)
    - Throws `std::invalid_argument` for invalid bases or characters
    - ```
      bigint num("FF", 16);
      std:cout << num; // Output: 255
      ```
5. Unary: `-`
    - Negates the `bigint` value
    - ```
      bigint num(123);
      std::cout << -num; // Output: -123
      ```
6. Pre/Post Increment/Decrement:
    - `bigint &operator++()`: Pre-increment
    - `bigint operator++(int)`: Post-increment
    - `bigint &operator--()`: Pre-decrement
    - `bigint operator--(int)`: Post-decrement
    - ```
      bigint num(10);
      ++num;
      std::cout << num; //Output: 11
      ```
7. Compound Assignment:
    - `bigint &operator+=(const bigint &rhs)`
    - `bigint &operator-=(const bigint &rhs)`
    - `bigint &operator*=(const bigint &rhs)`
    - `bigint &operator/=(const bigint &rhs)`
    - `bigint &operator%=(const bigint &rhs)`
  
    - E.g.
    - ```
      bigint num(10);
      num += bigint(5);
      std::cout << num; // Output: 15
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
