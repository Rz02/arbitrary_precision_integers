# Arbitrary Precision Integers

## Overview
The **bigint** class is designed for **arbitrary-precision integer arithmetic**. It allows for the manipulation of large integers, beyond the typical limits of standard integer types in C++, by storing the digits of the number in **a vector of uint8_t**. This class supports basic arithmetic operations like addition, subtraction, and multiplication, as well as comparison operators, making it suitable for applications requiring high-precision numbers.

## Features
Arbitrary Precision: 
- Supports integers of any size, limited only by system memory.

Signed Arithmetic: 
- Handles both positive and negative integers.

Basic Operations: 
- Supports addition, subtraction, multiplication, division, and negation.
- Supports increment and decrement operators in both pre- and post- forms.

Comparison: 
- Supports comparisons (`==, !=, <, <=, >, >=`).

Input/Output: 
- Can be initialized from strings or integers, and output to streams.

## Usage
- Note:

    Include the header file `#include "bigint.hpp"` in the program to use the `bigint` class.
- Constructors:

    1. **Default**: Create a `bigint` with a value of `0`: `bigint num;`
    2. **Integer**: Initialize a `bigint` from a 64-bit integer: `bigint num(123456789);`
    3. **String**: Initialize a `bigint` from a string of digits. Supports both positive and negative numbers: `bigint num("-123456789");`
- Operators:
    - Arithmetic:
        * `+`: add two `bigint` objects
        * `-`: subtract one `bigint` from another
        * `*`: multiply two `bigint` objects
        * `/`: divide one `bigint` from another
        * `%`: return the remainder of one `bigint` devided by another
    - Compound Assignment:
        * `+=, -=, *=, /=, %=`: Perform arithmetic operations and update the current object.
    - Unary:
        * `-`: Negates a `bigint` value
        * `++, --`: Increment and decrement operators (both pre- and post- forms)
    - Comparison:
        * `==, !=, <, <=, >, >=`: Compare two `bigint` objects
    - `<<` prints the value of the `bigint`
## Member Functions
### Public:
1. Default: `bigint()`
    - Create a `bigint` initialized to `0`
2. Integer: `bigint(int64_t num)`
    - Initialize a `bigint` from an integer
3. String: `bigint(const std::string &str)`
    - Convert a string representation into a `bigint`
4. Unary: `-`
    - Negates the `bigint` value
5. Pre/Post Increment/Decrement:
    - `bigint &operator++()`: Pre-increment
    - `bigint operator++(int)`: Post-increment
    - `bigint &operator--()`: Pre-decrement
    - `bigint operator--(int)`: Post-decrement
6. Compound Assignment:
    - `bigint &operator+=(const bigint &rhs)`
    - `bigint &operator-=(const bigint &rhs)`
    - `bigint &operator*=(const bigint &rhs)`

### Private:
1. `trim()`:
    - Remove leading zeros from the internal representation.
2. `add_vec(const std::vector<uint8_t> &a, const std::vector<uint8_t> &b)`:
    - Add two vectors.
3. `subtract_vec(const std::vector<uint8_t> &a, const std::vectro<uint8_t> &b)`:
    - Subtract one vector from another.
4. `abs_compare(const std::vectro<uint8_t> &a, const std::vector<uint8_t> &b)`:
    - Compare two vectors.
