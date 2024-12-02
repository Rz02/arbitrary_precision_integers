#include "bigint.hpp"
#include <iostream>
#include <stdexcept>
#include <vector>

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
            throw std::invalid_argument("Fail.");

        std::cout << "Testing string constructor: Pass.\n";
    }
    catch (const std::invalid_argument &e)
    {
        std::cout << e.what() << '\n';
    }
}

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

void test_increment_decrement_operators()
{
    try
    {
        bigint num(10);
        std::cout << "Initial num: " << num << '\n';

        // Test pre-increment
        bigint preIncrement = num;
        ++num;
        std::cout << "After ++num, num: " << num << '\n';
        if (num != preIncrement + 1)
            throw std::invalid_argument("Pre-increment failed.");

        // Test post-increment
        bigint postIncrement = num++;
        std::cout << "After num++, num: " << num << ", Post-incremented value: " << postIncrement << '\n';
        if (num != preIncrement + 2 || postIncrement != preIncrement + 1)
            throw std::invalid_argument("Post-increment failed.");

        // Test pre-decrement
        bigint preDecrement = num;
        --num;
        std::cout << "After --num, num: " << num << '\n';
        if (num != preDecrement - 1)
            throw std::invalid_argument("Pre-decrement failed.");

        // Test post-decrement
        bigint postDecrement = num--;
        std::cout << "After num--, num: " << num << ", Post-decremented value: " << postDecrement << '\n';
        if (num != preDecrement - 2 || postDecrement != preDecrement - 1)
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

void test_division()
{
    try
    {
        std::cout << "Testing Division: \n";

        bigint a(100);
        bigint b(7);
        if (!(a / b == 14))
            throw std::invalid_argument("Fail: Simple division.");

        bigint c(-100);
        if (!(c / b == -14))
            throw std::invalid_argument("Fail: Division by a negative number.");

        bigint d(100);
        bigint e(-7);
        if (!(d / e == -14))
            throw std::invalid_argument("Fail: Division with a negative divisor.");

        bigint f(-100);
        bigint g(-7);
        if (!(f / g == 14))
            throw std::invalid_argument("Fail: Both operands negative.");

        bigint h(12345);
        if (!(h / 1 == 12345))
            throw std::invalid_argument("Fail: Division by 1.");
        if (!(h / -1 == -12345))
            throw std::invalid_argument("Fail: Division by -1.");

        bigint i(100);
        bigint j(7);
        i /= j;
        if (i != 14)
            throw std::invalid_argument("Fail: /= operator.");

        bigint l("987654321987654321");
        bigint m("123456789123456789");
        if (!(l / m == 8))
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

void test_modulus()
{
    try
    {
        std::cout << "Testing Modulus: \n";

        // Test case 1: Simple modulus
        bigint a(100);
        bigint b(7);
        if (!(a % b == 2))
            throw std::invalid_argument("Fail: Simple modulus.");

        // Test case 2: Modulus with a negative divisor
        bigint c(100);
        bigint d(-7);
        if (!(c % d == 2))
            throw std::invalid_argument("Fail: Modulus with a negative divisor.");

        // Test case 3: Both operands negative
        bigint e(-100);
        bigint f(-7);
        if (!(e % f == -2))
            throw std::invalid_argument("Fail: Both operands negative.");

        // Test case 4: Large number modulus
        bigint g("987654321987654321");
        bigint h("123456789123456789");
        if (!(g % h == bigint("9000000009")))
            throw std::invalid_argument("Fail: Large number modulus.");

        // Test case 5: Assignment modulus operator
        bigint i(100);
        bigint j(7);
        i %= j;
        if (i != 2)
            throw std::invalid_argument("Fail: %= operator.");

        // Test case 6: Modulus by zero
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
    test_division();
    test_modulus();
}
