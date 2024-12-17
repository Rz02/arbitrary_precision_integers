/**
 * @file test.cpp
 * @author Zicheng Zhao (zhaoz149@mcmaster.com)
 * @brief
 * @version 0.1
 * @date 2024-12-17
 *
 * @copyright Copyright (c) 2024
 *
 */
#include "bigint.hpp"
#include <iostream>
#include <stdexcept>
#include <vector>

/**
 * @brief Tests the default constructor of the `bigint` class.
 *
 * This test checks if the default constructor initializes the `bigint` object
 * correctly to represent the value zero. It also verifies exception handling for
 * any unexpected behavior.
 *
 */
void test_default_constructor()
{
    try
    {
        bigint num;
        std::cout << "Testing default constructor: ";
        if (num == 0)
            std::cout << "Pass.\n";
        else
            throw std::invalid_argument("Fail.");
    }
    catch (const std::invalid_argument &e)
    {
        std::cout << e.what() << '\n';
    }
}

/**
 * @brief Tests the integer constructor of the `bigint` class.
 *
 * This test checks if the constructor correctly initializes `bigint` objects
 * with positive, negative, and zero values when provided with integer inputs.
 * It also validates exception handling for unexpected behaviors.
 *
 */
void test_int_constructor()
{
    try
    {
        bigint num1(123);
        bigint num2(-456);
        bigint num3(0);
        std::cout << "Testing int constructor: ";
        if (num1 == 123 && num2 == -456 && num3 == 0)
            std::cout << "Pass.\n";
        else
            throw std::invalid_argument("Fail.");
    }
    catch (const std::invalid_argument &e)
    {
        std::cout << e.what() << '\n';
    }
}

/**
 * @brief Tests the string constructor of the `bigint` class.
 *
 * This test checks various cases for constructing a `bigint` object from strings:
 *
 * - Valid positive and negative number strings.
 * - Edge cases such as zero, leading zeros, and "negative zero."
 * - Handling invalid input like empty strings, non-digit characters, spaces,
 *   and special characters.
 * - Very large numbers to test the robustness of the constructor.
 *
 * It also verifies correct exception handling for invalid inputs.
 *
 */
void test_string_constructor()
{
    try
    {
        // Test valid positive number
        bigint num1("12345");
        std::cout << "num1 (Expected: 12345): " << num1 << '\n';
        if (num1 != 12345)
            throw std::invalid_argument("Fail.");

        // Test valid negative number
        bigint num2("-9876");
        std::cout << "num2 (Expected: -9876): " << num2 << '\n';
        if (num2 != -9876)
            throw std::invalid_argument("Fail.");

        // Test invalid empty string
        std::string input = "";
        try
        {
            bigint num3(input);
            throw std::invalid_argument("Fail.");
        }
        catch (const std::invalid_argument &e)
        {
            std::cout << "Caught expected exception for empty string: " << e.what() << '\n';
        }

        // Test invalid string with non-digit characters
        input = "12a45";
        try
        {
            bigint num4(input);
            throw std::invalid_argument("Fail.");
        }
        catch (const std::invalid_argument &e)
        {
            std::cout << "Caught expected exception for invalid character: " << e.what() << '\n';
        }

        // Test invalid string with spaces
        input = "12 345";
        try
        {
            bigint num5(input);
            throw std::invalid_argument("Fail.");
        }
        catch (const std::invalid_argument &e)
        {
            std::cout << "Caught expected exception for invalid character (spaces): " << e.what() << '\n';
        }

        // Test invalid string with special characters
        input = "12@345";
        try
        {
            bigint num6(input);
            throw std::invalid_argument("Fail.");
        }
        catch (const std::invalid_argument &e)
        {
            std::cout << "Caught expected exception for invalid character (special chars): " << e.what() << '\n';
        }

        // Test string "0" (valid zero input)
        input = "0";
        bigint num7(input);
        std::cout << "Expected 0: " << num7 << '\n';
        if (num7 != 0)
            throw std::invalid_argument("Fail. String zero");

        // Test large number
        bigint num8("123456789123456789123456789");
        if (!(num8 == bigint("123456789123456789123456789")))
            throw std::invalid_argument("Fail. Large number");

        // Test leading zero trimming
        bigint num9("0000123456789");
        if (!(num9 == 123456789))
            throw std::invalid_argument("Fail. Zero trimming");

        // Test Negative zero
        bigint num0("-0");
        if (!(num0 == 0))
            throw std::invalid_argument("Fail. Negative Zero");

        std::cout << "Testing string constructor: Pass.\n";
    }
    catch (const std::invalid_argument &e)
    {
        std::cout << e.what() << '\n';
    }
}

/**
 * @brief Tests the stream insertion operator (<<) for the `bigint` class.
 *
 * This test verifies that the `bigint` objects are correctly formatted and
 * displayed as strings when inserted into an output stream. Both positive
 * and negative numbers are tested to ensure proper formatting.
 *
 */
void test_stream_insertion()
{
    try
    {
        bigint num1(12345);
        bigint num2(-9876);
        std::cout << "Testing stream insertion: \n";
        std::cout << "Expected: 12345, Output: ";
        if (num1 == 12345)
            std::cout << num1 << '\n';
        else
            throw std::invalid_argument("Wrong format");
        std::cout << "Expected: -9876, Output: ";
        if (num2 == -9876)
            std::cout << num2 << '\n';
        else
            throw std::invalid_argument("Wrong format");
    }
    catch (const std::invalid_argument &e)
    {
        std::cout << e.what() << '\n';
    }
}

/**
 * @brief Tests the comparison operators for the `bigint` class.
 *
 * This test validates the correctness of all comparison operators (`==`, `!=`,
 * `<`, `<=`, `>`, `>=`) for `bigint`. Positive, negative, and equivalent
 * values are compared to ensure consistent and accurate behavior.
 *
 */
void test_comparison_operators()
{
    try
    {
        bigint num1(12345);
        bigint num2(9876);
        bigint num3(12345);
        bigint num4(-12345);

        std::cout << "Testing comparison operators: ";
        if (num1 == num3 && num1 != num2 && num1 > num2 && num1 >= num3 && num2 < num1 && num2 <= num1)
            std::cout << "Pass.\n";
        else
            throw std::invalid_argument("Fail.");
    }
    catch (const std::invalid_argument &e)
    {
        std::cout << e.what() << '\n';
    }
}

/**
 * @brief Tests the arithmetic operators for the `bigint` class.
 *
 * This test ensures that addition, subtraction, multiplication, division,
 * and modulo operations work correctly for `bigint` objects. The results are
 * compared with expected values for a variety of inputs.
 *
 */
void test_arithmetic_operators()
{
    try
    {
        bigint num1(123);
        bigint num2(456);
        bigint result_add = num1 + num2;
        bigint result_sub = num1 - num2;
        bigint result_mul = num1 * num2;
        bigint result_div = num2 / num1;
        bigint result_mod = num2 % num1;

        std::cout << "Testing arithmetic operators: ";
        if (result_add == 579 && result_sub == -333 && result_mul == 56088 && result_div == 3 && result_mod == 87)
            std::cout << "Pass.\n";
        else
            throw std::invalid_argument("Fail.");
    }
    catch (const std::invalid_argument &e)
    {
        std::cout << e.what() << '\n';
    }
}

/**
 * @brief Tests compound assignment operators for the `bigint` class.
 *
 * This test validates the correctness of the compound assignment operators
 * (`+=`, `-=`, `*=`, `/=`, `%=`) when used with `bigint` objects. The
 * operators are tested with various inputs to ensure expected results.
 *
 */
void test_compound_assignment_operators()
{
    try
    {
        bigint num1(123);
        bigint num2(456);
        num1 += num2;
        num2 -= num1;
        num1 *= bigint(2);
        num2 /= bigint(2);
        num2 %= bigint(60);

        std::cout
            << "Testing compound assignment operators: ";
        if (num1 == 1158 && num2 == -1)
            std::cout << "Pass.\n";
        else
            throw std::invalid_argument("Fail.");
    }
    catch (const std::invalid_argument &e)
    {
        std::cout << e.what() << '\n';
    }
}

/**
 * @brief Tests the unary negation operator (-) for the `bigint` class.
 *
 * This test verifies that negating a positive `bigint` correctly converts
 * it to a negative value and vice versa. It also ensures that negating
 * zero has no effect.
 *
 */
void test_unary_negation()
{
    try
    {
        bigint num1(123);
        bigint num2(-456);
        bigint result = -num1;

        std::cout << "Testing unary negation operator: ";
        if (result == -123 && -num2 == 456)
            std::cout << "Pass.\n";
        else
            throw std::invalid_argument("Fail.");
    }
    catch (const std::invalid_argument &e)
    {
        std::cout << e.what() << '\n';
    }
}

/**
 * @brief Tests the increment and decrement operators for the `bigint` class.
 *
 * This test verifies the correct operation of pre-increment (`++num`), post-increment (`num++`),
 * pre-decrement (`--num`), and post-decrement (`num--`) operators for the `bigint` class.
 * The expected behavior is that the values increase or decrease as expected with both
 * pre- and post-increment/decrement variants.
 *
 */
void test_increment_decrement_operators()
{
    try
    {
        bigint num(10);
        std::cout << "Initial num: " << num << '\n';

        // Test pre-increment
        bigint preInc = num;
        ++num;
        std::cout << "After ++num, num: " << num << '\n';
        if (num != preInc + 1)
            throw std::invalid_argument("Pre-increment failed.");

        // Test post-increment
        bigint postInc = num++;
        std::cout << "After num++, num: " << num << ", Post-incremented value: " << postInc << '\n';
        if (num != preInc + 2 || postInc != preInc + 1)
            throw std::invalid_argument("Post-increment failed.");

        // Test pre-decrement
        bigint preDec = num;
        --num;
        std::cout << "After --num, num: " << num << '\n';
        if (num != preDec - 1)
            throw std::invalid_argument("Pre-decrement failed.");

        // Test post-decrement
        bigint postDec = num--;
        std::cout << "After num--, num: " << num << ", Post-decremented value: " << postDec << '\n';
        if (num != preDec - 2 || postDec != preDec - 1)
            throw std::invalid_argument("Post-decrement failed.");

        // Final check for correctness
        std::cout << "Testing increment and decrement operators: ";
        if (num == 10)
            std::cout << "Pass.\n";
        else
            throw std::invalid_argument("Final value mismatch.");
    }
    catch (const std::invalid_argument &e)
    {
        std::cout << e.what() << '\n';
    }
}

/**
 * @brief Test the addition operator for the `bigint` class.
 *
 * This function verifies the correctness of addition with edge cases,
 * including positive numbers, negative numbers, zero, and large numbers.
 *
 */
void test_addition()
{
    std::cout << "Testing Addition: \n";

    bigint a(100);
    bigint b(-50);
    bigint c(0);
    bigint d("123456789123456789");
    bigint e("876543210876543210");

    if (!(a + b == 50))
        throw std::invalid_argument("Fail: Positive + Negative.");
    if (!(a + c == 100))
        throw std::invalid_argument("Fail: Addition with zero.");
    if (!(d + e == bigint("999999999999999999")))
        throw std::invalid_argument("Fail: Addition with large numbers.");

    std::cout << "Pass.\n";
}

/**
 * @brief Test the subtraction operator for the `bigint` class.
 *
 * This function verifies the correctness of subtraction with edge cases,
 * including positive numbers, negative numbers, zero, and results that are negative.
 *
 */
void test_subtraction()
{
    std::cout << "Testing Subtraction: \n";

    bigint a(100);
    bigint b(-50);
    bigint c(0);
    bigint d("123456789123456789");
    bigint e("123456789123456788");

    if (!(a - b == 150))
        throw std::invalid_argument("Fail: Positive - Negative.");
    if (!(a - c == 100))
        throw std::invalid_argument("Fail: Subtraction with zero.");
    if (!(e - d == -1))
        throw std::invalid_argument("Fail: Subtraction resulting in negative.");

    std::cout << "Pass.\n";
}

/**
 * @brief Test the multiplication operator for the `bigint` class.
 *
 * This function verifies the correctness of multiplication with edge cases,
 * including positive numbers, negative numbers, zero, and large numbers.
 *
 */
void test_multiplication()
{
    std::cout << "Testing Multiplication: \n";

    bigint a(10);
    bigint b(-5);
    bigint c(0);
    bigint d("123456789");
    bigint e("987654321");

    if (!(a * b == -50))
        throw std::invalid_argument("Fail: Positive * Negative.");
    if (!(a * c == 0))
        throw std::invalid_argument("Fail: Multiplication by zero.");
    if (!(b * b == 25))
        throw std::invalid_argument("Fail: Negative * Negative.");
    if (!(d * e == bigint("121932631112635269")))
        throw std::invalid_argument("Fail: Multiplication of large numbers.");

    std::cout << "Pass.\n";
}

/**
 * @brief Tests the division operator for the `bigint` class.
 *
 * This test validates the correctness of the division operator (`/`) for various
 * `bigint` inputs, including positive, negative, and large numbers. It also checks
 * the division by zero scenario to ensure an exception is thrown.
 *
 */
void test_division()
{
    try
    {
        std::cout << "Testing Division: \n";

        bigint a(100);
        bigint b(7);
        bigint c(-100);
        bigint d(-7);
        bigint e(12345);
        bigint f("987654321987654321");
        bigint g("123456789123456789");

        if (!(a / b == 14))
            throw std::invalid_argument("Fail: Simple division.");

        if (!(c / b == -14))
            throw std::invalid_argument("Fail: Division by a negative number.");

        if (!(a / d == -14))
            throw std::invalid_argument("Fail: Division with a negative divisor.");

        if (!(c / d == 14))
            throw std::invalid_argument("Fail: Both operands negative.");

        if (!(e / 1 == 12345))
            throw std::invalid_argument("Fail: Division by 1.");
        if (!(e / -1 == -12345))
            throw std::invalid_argument("Fail: Division by -1.");

        a /= b;
        if (a != 14)
            throw std::invalid_argument("Fail: /= operator.");

        if (!(f / g == 8))
            throw std::invalid_argument("Fail: Division for large numbers.");

        try
        {
            bigint num1(100);
            bigint zero(0);
            num1 / zero;
            throw std::invalid_argument("Fail: Division by zero did not throw an exception.");
        }
        catch (const std::invalid_argument &e)
        {
            std::cout << "Caught expected exception for division by zero: " << e.what() << '\n';
        }

        std::cout << "Pass.\n";
    }
    catch (const std::invalid_argument &e)
    {
        std::cout << e.what() << '\n';
    }
}

/**
 * @brief Tests the modulus operator for the `bigint` class.
 *
 * This test validates the correctness of the modulus operator (`%`) for various
 * `bigint` inputs, including positive, negative, and large numbers. It also ensures
 * that modulus by zero throws the expected exception.
 *
 */
void test_modulus()
{
    try
    {
        std::cout << "Testing Modulus: \n";

        bigint a(100);
        bigint b(7);
        bigint c(-7);
        bigint d(-100);
        bigint e("987654321987654321");
        bigint f("123456789123456789");

        if (!(a % b == 2))
            throw std::invalid_argument("Fail: Simple modulus.");

        if (!(a % c == 2))
            throw std::invalid_argument("Fail: Modulus with a negative divisor.");

        if (!(d % c == -2))
            throw std::invalid_argument("Fail: Both operands negative.");

        if (!(e % f == bigint("9000000009")))
            throw std::invalid_argument("Fail: Large number modulus.");

        a %= b;
        if (a != 2)
            throw std::invalid_argument("Fail: %= operator.");

        try
        {
            bigint num1(100);
            bigint zero(0);
            num1 % zero;
            throw std::invalid_argument("Fail: Modulus by zero did not throw an exception.");
        }
        catch (const std::invalid_argument &e)
        {
            std::cout << "Caught expected exception for modulus by zero: " << e.what() << '\n';
        }

        std::cout << "Pass.\n";
    }
    catch (const std::invalid_argument &e)
    {
        std::cout << e.what() << '\n';
    }
}

/**
 * @brief Test the constructor of the bigint class that takes a string and a base.
 *
 * This function tests various valid and invalid cases for the `bigint` constructor,
 * which creates a `bigint` object from a string representation in a specified base
 * (from base 2 to base 36).
 *
 */
void test_string_base_constructor()
{
    try
    {
        std::cout << "Testing String Base Constructor:\n";

        // Test case 1: Valid hexadecimal input (Base 16)
        bigint num1("FF", 16);
        if (num1 != 255)
        {
            throw std::invalid_argument("Fail: FF in base 16 should be 255.");
        }

        // Test case 2: Valid decimal input (Base 10)
        bigint num2("100", 10);
        if (num2 != 100)
        {
            throw std::invalid_argument("Fail: 100 in base 10 should be 100.");
        }

        // Test case 3: Valid binary input (Base 2)
        bigint num3("1101", 2);
        if (num3 != 13)
        {
            throw std::invalid_argument("Fail: 1101 in base 2 should be 13.");
        }

        // Test case 4: Valid base 36 input (Base 36)
        bigint num4("Z", 36); // 'Z' in base 36 is 35
        if (num4 != 35)
        {
            throw std::invalid_argument("Fail: Z in base 36 should be 35.");
        }

        // Test case 5: Valid mixed case base 36 input (Base 36)
        bigint num5("aB", 36); // 'a' is 10 and 'B' is 11, so "aB" in base 36 is 10 * 36 + 11 = 371
        if (num5 != 371)
        {
            throw std::invalid_argument("Fail: aB in base 36 should be 371.");
        }

        // Test case 6: Negative number in decimal (Base 10)
        bigint num6("-100", 10);
        if (num6 != -100)
        {
            throw std::invalid_argument("Fail: -100 in base 10 should be -100.");
        }

        // Test case 7: Valid large number in base 10
        bigint num7("123456789123456789", 10);
        if (num7 != 123456789123456789)
        {
            throw std::invalid_argument("Fail: 123456789123456789 in base 10 is incorrect.");
        }

        // Test case 8: Invalid character in base 16 (Base 16)
        try
        {
            bigint num8("G1", 16); // 'G' is invalid in base 16
            throw std::invalid_argument("Fail: Invalid character 'G' in base 16 should throw an exception.");
        }
        catch (const std::invalid_argument &e)
        {
            std::cout << "Caught expected exception for invalid character in base 16: " << e.what() << '\n';
        }

        // Test case 9: Invalid character for base 10
        try
        {
            bigint num9("19A", 10); // 'A' is invalid in base 10
            throw std::invalid_argument("Fail: Invalid character 'A' in base 10 should throw an exception.");
        }
        catch (const std::invalid_argument &e)
        {
            std::cout << "Caught expected exception for invalid character in base 10: " << e.what() << '\n';
        }

        // Test case 10: Base less than 2
        try
        {
            bigint num10("101", 1); // Base 1 is not valid
            throw std::invalid_argument("Fail: Base 1 is invalid.");
        }
        catch (const std::invalid_argument &e)
        {
            std::cout << "Caught expected exception for invalid base 1: " << e.what() << '\n';
        }

        // Test case 11: Base greater than 36
        try
        {
            bigint num11("ZZ", 37); // Base 37 is invalid
            throw std::invalid_argument("Fail: Base 37 is invalid.");
        }
        catch (const std::invalid_argument &e)
        {
            std::cout << "Caught expected exception for invalid base 37: " << e.what() << '\n';
        }

        std::cout << "Pass!\n";
    }
    catch (const std::invalid_argument &e)
    {
        std::cout << "Test failed: " << e.what() << '\n';
    }
}

/**
 * @brief Main function to execute all tests.
 *
 * This function runs the individual test functions for the `bigint` class to
 * verify that the class behaves correctly for various operators and constructors.
 *
 * @return int Exit status.
 */
int main()
{
    test_default_constructor();
    test_int_constructor();
    test_string_constructor();
    test_stream_insertion();
    test_comparison_operators();
    test_arithmetic_operators();
    test_compound_assignment_operators();
    test_unary_negation();
    test_increment_decrement_operators();
    test_addition();
    test_subtraction();
    test_multiplication();
    test_division();
    test_modulus();
    test_string_base_constructor();
}
