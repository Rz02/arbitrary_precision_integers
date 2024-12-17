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
  - `friend std::ostream &operator<<(std::ostream &out, const bigint &num)`
  - ```
    // E.g.
    bigint num(12345);
    std::cout << num; // Output: 12345
    ```
  - Mechanism:
    - Firstly, if the number is negative, output the minus sign.
    - Then, iterate through the digits stored in the vector in reversed order, from the most significant to the least significant digit.
    - Finally, we return the result.

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
    - Mechanism:
        - For `==`:
            - We check whether two bigint has the same sign and the same absolute value
        - For `!=`:
            - It's the negation of `==`
        - For `<`:
            - We firstly check whether two bigint has the different sign, since a negative number is always less than a positive one.
            - Then, we compare the size of the vectors, since larger size implies a larger absolute value.
            - After these checking, we do comparison digit-by-digit starting from the most significant digit.
            - If all digits are equal, the numbers are equal.
        - For `<=`:
            - We check whether the current bigint is less than or equal to the given bigint.
        - For `>`:
            - It's the negation of `<=`
        - For `>=`
            - It's the negation of `<`

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
    - Mechanism:
        - For `+`:
            - We firstly check whether the signs are the same, we add the absolute values if so. 
            - Otherwise, we perform subtraction of the absolute values.
        - For `-`:
            - We firstly check whether the signs differ, we perform addition of the absolute values if so.
            - Otherwise, we subtract the absolute value of the given bigint from the current bigint's one.
        - For `*`:
            - We firstly initialize a vector to hold the product, with its size set for maximum possible digits, which is the sum of the current bigint's size and the given bigint's size.
            - Then we perform digit-by-digit multiplication by handling carry propagation, which is similar to manual calculation.
            - Afterwards, we set the sign of the result based on the operands.
        - For `/`:
            - We firstly handle the case of the division by zero.
            - Then, we initialize both dividend (current bigint) and divisor(given bigint) positive for calculation.
            - If the divisor is greater than the dividend, then the result is zero.
            - Then, we initialize variables for the quotient and the current value being divided.
            - Now we can perform long division, which is similar to manual calculation.
            - In the end, we reverse the quotient vector and set the correct sign.
        - For `%`:
            - We firstly handle the case of the division by zero.
            - Then, we initialize both dividend (current bigint) and divisor (given bigint) positive for calculation.
            - If the dividend is smaller than the divisor, the result return the dividend.
            - If not, now we can perform modulus operation, which has a similar theory as long division, and is also similar to manual calculation.
            - In the end, we adjust the correct sign for the remainder.

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
    - Mechanism:
        - Compound operation by leveraging the corresponding operator defined above between the current bigint and the given bigint. 

4. Unary: `-`
    - `bigint operator-() const`
    - Negates the `bigint` value
    - ```
      // E.g.
      bigint num(123);
      std::cout << -num; // Output: -123
      ```
    - Mechanism:
        - Make the given bigint changing its sign.

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
    - Mechanism:
        - For pre-increment, we modify the current bigint by adding/subtracting 1.
        - For post-increment, we set up a copy of the current bigint before the operation.

6. Default: `bigint()`
    - Create a `bigint` initialized to `0`
    - ```
      // E.g.
      bigint num;
      std::cout << num; // Output: 0
      ```
    - Mechanism:
        - We initialize by setting its vector as 0: `bigint() : vec({0}) {}`, where `vec` is set in *Private* section.

7. Integer: `bigint(int64_t num)`
    - Initialize a `bigint` from an integer
    - ```
      // E.g.
      bigint num(12345);
      std::cout << num; // Output: 12345
      ```
    - Mechanism:
        - We firstly check the sign of the integer. If it's negative, set the status of sign for the bigint as *negative*, and flip the sign of the number for representing its absolute value.
        - We also check whether it's zero or not.
        - We then convert the number to a vector of digits in reverse order by using `num % 10` for `push_back` and `num /= 10` for iterating in while loop as long as `num > 0`

8. String: `bigint(const std::string &str)`
    - Convert a string representation into a `bigint`
    - Supports both positive and negative values
    - Throws `std::invalid_argument` for invalid input
    - ```
      // E.g.
      bigint num("-987654321");
      std::cout << num; // Output: -987654321
      ```
    - Mechanism:
        - We firstly check whether the string is empty or not.
        - We then check if the string represents a negative number. If so, we will set the status of sign for the bigint as *negative*, and we will proceed the string starting from the first index, a.k.a. the second parameter.
        - We also check whether the string contains invalid characters or not by using `std::isdigit`.
        - If everything is alright, we use `char - '0'`, where `char` is the element of the string. This subtracts from the character to which string is pointing the ASCII code of the character '0'. So, '0' - '0' gives you 0 and so on and '9' - '0' gives you 9. (https://stackoverflow.com/questions/12927087/what-does-string-0-do-string-is-a-char) Then we use `static_cast` to convert it to `uint8_t` to fit it in the vector
        - In the end, we use the helper function `trim()` to trim leading zero.

9. String with Base: `bigint(const std::string &str, int base)`
    - Constructs a `bigint` from a string in the specified base (2 to 36)
    - Throws `std::invalid_argument` for invalid bases or characters
    - ```
      // E.g.
      bigint num("FF", 16);
      std:cout << num; // Output: 255
      ```
    - Mechanism:
        - We firstly check whether the base is in a reasonable range.
        - We then keep track of the sign by firstly initialize the bigint as positive, then we check if the number from the string is negative or not. If so, we will set the sign as negative, then skip to the second index for processing. 
            - *Specifically*, an extra temporary sign parameter is set to track the status of `is_negative`, since the default setting of the bigint's sign is *positive*, which will influence the status of `is_negative`, one of the *private* variable for default setting.
        - Then, we initialize the current bigint for preventing from garbage value. Afterward, we convert the string by specifically treating three different scenarios:
            - *0 - 9*
            - *a - z*
            - *A - Z*
        Accordingly, except the first scenario, an `+ 10` is needed after `char - '0'` for correctly tracking the value based on the rules of counting.
        - Afterward, we double check to see whether the digit is smaller than the base.
        - In the end, we use the temporary sign parameter to confirm the bigint's sign.

### Private:
1. `bool is_negative = false;`
    - Indicates whether the bigint object represents a negative number.
    - Default is false, meaning the number is positive.
2. `std::vector<uint8_t> vec = std::vector<uint8_t>();`
    - Stores the digits of the bigint object in reverse order.
    - Each element represents a single digit (0–9) of the number, with the least significant digit at index 0.
    - Default is an empty vector.
3. `bigint(bool negative, const std::vector<uint8_t> &vector) : is_negative(negative), vec(vector) { trim(); }`
    - Construct a new bigint object from a sign indicator and a vector of digits.
    - This constructor initializes the bigint object directly with the provided sign and digit vector.
4. `trim()`:
    - Remove leading zeros from the internal representation.
    - Mechanism:
        - Since the vector records the number in a reverse order, so we are facing two scenarios:
            - If the size of the vector is bigger than 1 and has 0 at the end, we will get rid of the 0.
            - If the size of the vector is 1 and is 0 at the position, we just need to ensure that its sign is positive for a positive 0.
5. `add_vec(const std::vector<uint8_t> &a, const std::vector<uint8_t> &b)`:
    - Add two vectors.
    - Mechanism:
        - We firstly initialize a vector for storing the result. We also select the largest size of the vector as the max times of iteration. We also set up a carry variable for handling carry propagation.
        - The loop iterates through each digit of the two input vectors, as long as there are digits to process in either vector or there is a carry that needs to be handled.
        - Inside the loop, the parameter sum starts with the current carry value. If i is within bounds of first vector, add corresponding element to sum. Same theory for the second vector.
        - We then convert the `sum % 10` in the corresponding index, and `sum / 10` as carry for iteration.
        - In the end, after the looping, we obtain the result.

6. `subtract_vec(const std::vector<uint8_t> &a, const std::vector<uint8_t> &b)`:
    - Subtract one vector from another.
    - Mechanism:
        - We firstly initialize a vector for storing the result. We also set up a borrow variable for handling borrow propagation.
        - We select the size of the minuend vector as the max times of iteration.
        - Inside the loop, the parameter diff starts with the subtraction between the corresponding element and the borrow variable, similar to how do we handle manual calculation in subtraction.
        - Additionally, there might be extra zero that need to be trimmed. A function using `pop_back()` is explicitly set for double check. 
7. `abs_compare(const std::vector<uint8_t> &a, const std::vector<uint8_t> &b)`:
    - Compare two vectors.
    - Mechanism:
        - For the *first vector a* and the *second vector b*, return:
            - 1 if |a| > |b|
            - -1 if |a| < |b|
            - 0 if |a| == |b|
