/**
 * @file BigInt.cpp
 * @author Gabrielle Ching-Johnson
 * @brief C++ code file for arbitrary precision integer arithmetic
 * @version 0.2
 * @date Dec 19, 2021
 */

#include <iostream>
#include <vector>
#include <cstdint>
#include <sstream>
#include "big_int.hpp"
using namespace std;

/******************************* Constructors ******************************/
big_int::big_int() : coefficient({0}){};

big_int::big_int(const int64_t &integer)
{
    // Add the coefficients/digits for the integer in base 2^32
    uint64_t value = integer;
    // If value is negative save as a positive and update sign
    if (integer < 0)
    {
        integer_sign = sign::NEGATIVE;
        value = -integer;
    }

    // Divide by base of 2^32 and add remainder to the coefficient vector of digits
    uint32_t remainder = (uint32_t)(value % base);
    coefficient.push_back(remainder);
    while (value > base)
    {
        value = value / base;
        remainder = (uint32_t)(value % base); // remainder always < base
        coefficient.push_back(remainder);
    }
}

big_int::big_int(const string &integer) : coefficient((1))
{
    string integer_temp = integer;
    // Check to see if 1st digit is a "+" or "-" sign and remove
    if (integer[0] == '+')
    {
        integer_sign = sign::POSITIVE;
        integer_temp.erase(0, 1);
    }
    else if (integer[0] == '-')
    {
        integer_sign = sign::NEGATIVE;
        integer_temp.erase(0, 1);
    }

    // For each digit character in the string multiply the big integer by 10 and then add the digit value
    for (const char &digit : integer_temp)
    {
        // Check to make sure digit is from 0 to 9
        if (isdigit(digit) == 0)
        {
            throw invalid_string_integer();
        }

        multiply_32(10);
        add_32(digit - '0'); // the character digit will have ascii value thus this value minus the value at 0 will give base 10 digit value
    }
}

big_int::big_int(const big_int &big_integer)
{
    // Copy the digits of the argument big integer
    for (const uint32_t &digits : big_integer.coefficient) // ***TODO: See if breaks encapsulation***
    {
        coefficient.push_back(digits);
    }

    // update the sign of the new big int to that of the argument
    integer_sign = big_integer.get_sign();
}

big_int::big_int(const vector<uint32_t> &vec)
{
    // Reverse the vector such that the most significant digit is at the last index
    coefficient.reserve(vec.size());
    for (uint64_t i = vec.size(); i > 0; i--)
    {
        coefficient.push_back(vec[i - 1]);
    }
}

/******************************** Public Functions ******************************/

void big_int::negate()
{
    // Change sign flag
    if (integer_sign == sign::POSITIVE)
    {
        integer_sign = sign::NEGATIVE;
    }
    else
    {
        integer_sign = sign::POSITIVE;
    }
}

const uint32_t &big_int::at(const uint64_t &index) const
{
    return coefficient.at(index);
}

uint64_t big_int::coefficient_size() const
{
    return coefficient.size();
}

const sign &big_int::get_sign() const
{
    return integer_sign;
}

// Referenced: https://en.cppreference.com/w/cpp/language/operators
big_int &big_int::operator=(const big_int &big_integer)
{

    // Self assignment
    if (this == &big_integer)
    {
        return *this;
    }

    // Set the coefficient and integer_sing of this object to that of argument
    coefficient = big_integer.coefficient;
    integer_sign = big_integer.get_sign();

    // return object
    return *this;
}
/******************************* Private Functions ******************************/
void big_int::multiply_32(const uint32_t &integer)
{
    uint64_t carry = 0;
    for (uint32_t &digit : coefficient)
    {
        // Multiply each digit with the integer and add any carry from previous multiplication
        uint64_t temp = (digit * (uint64_t)integer) + carry;
        digit = (uint32_t)(temp & (uint64_t)UINT32_MAX); //Keep the lower 32 bits as the value for the coefficient digit
        carry = temp >> 32;                              // Assign value of the carry by shifting temp value to the right by 32;
    }

    // Check to see if carry at the end is 0, if not add new coefficient to vector of coefficients
    if (carry != 0)
    {
        coefficient.push_back((uint32_t)carry);
    }
}

void big_int::add_32(const uint32_t &integer)
{
    // Add integer to only the very first coefficient (since the integer is only 32 bits)
    uint64_t temp = coefficient[0] + (uint64_t)integer;
    coefficient[0] = (uint32_t)(temp & (uint64_t)UINT32_MAX); // keep the low 32 bits
    uint64_t carry = temp >> 32;                              // shift temp to the right by 32 bits to get the carry

    // Check if a carry exists and update next coefficient until carry = 0 or until the end of the coefficient vector
    for (uint64_t i = 1; i < coefficient.size() && carry != 0; i++)
    {
        uint64_t temp = coefficient[i] + carry;
        coefficient[i] = (uint32_t)(temp & (uint64_t)UINT32_MAX);
        carry = temp >> 32;
    }

    // If the carry is not 0, need to add a new coefficient with the value of carry
    if (carry != 0)
    {
        coefficient.push_back((uint32_t)carry);
    }
}

void big_int::divide_32(const uint32_t &integer)
{

    uint64_t remainder = 0;
    // Start with most significant digit. Take each digit, add it to the remainder(from previous division) times the base and then divide by integer
    for (uint64_t i = coefficient.size(); i > 0; i--)
    {
        uint64_t temp = (base * remainder) + coefficient[i - 1];
        coefficient[i - 1] = (uint32_t)(temp / integer);
        remainder = temp % integer;
    }
}

uint32_t big_int::remainder_32(const uint32_t &integer) const
{
    uint64_t remainder = 0;
    // Start with most significant digit. Take each digit, add it to the remainder(from previous division) times the base and then divide by integer
    for (uint64_t i = coefficient.size(); i > 0; i--)
    {
        uint64_t temp = (base * remainder) + coefficient[i - 1];
        remainder = temp % integer;
    }
    // remainder will always be less than integer thus cast as 32 bit
    return (uint32_t)remainder;
}

vector<uint32_t> big_int::radix_complement() const
{
    vector<uint32_t> complement;
    // Ged the radix - 1
    uint32_t radix_minus1 = (uint32_t)(base - 1);

    // Find radix - 1 complement and then add 1 to get radix complement
    for (const uint32_t &digit : coefficient)
    {
        complement.push_back(radix_minus1 - digit);
    }

    // Add 1 to the big int and check carry
    uint64_t temp = complement[0] + 1;
    complement[0] = (uint32_t)(temp & (uint64_t)UINT32_MAX); // keep the low 32 bits
    uint64_t carry = temp >> 32;

    // If a carry exists add it to the next element in vector if it exist and recheck
    for (uint64_t i = 1; i < complement.size() && carry != 0; i++)
    {
        uint64_t temp = complement[i] + carry;
        complement[i] = (uint32_t)(temp & (uint64_t)UINT32_MAX); // keep the low 32 bits
        carry = temp >> 32;                                      // shift temp to the right by 32 bits to get the carry bit
    }

    // If the carry is not 0, need to add a new element with the value of carry
    if (carry != 0)
    {
        complement.push_back((uint32_t)carry);
    }

    return complement;
}

void big_int::expand(uint64_t const &num_zeros)
{
    for (uint64_t i = 0; i < num_zeros; i++)
    {
        coefficient.push_back(0);
    }
}

void big_int::shrink()
{
    // Do not shrink if value is 0 with only one digit
    if (coefficient.size() > 1)
    {
        // Remove all leading zeros until reach a digit that is not zero
        bool number_reached = false;
        for (uint64_t i = coefficient.size() - 1; i > 0 && !number_reached; i--)
        {
            if (coefficient[i] == 0)
            {
                coefficient.erase(coefficient.begin() + i);
            }
            else
            {
                number_reached = true;
            }
        }
    }
}

/******************************* Friend Functions ******************************/

big_int operator+(const big_int &int_a, const big_int &int_b)
{
    big_int sum;
    // Both integers are negative case
    if (int_a.get_sign() == sign::NEGATIVE && int_b.get_sign() == sign ::NEGATIVE)
    {
        // add the two big integers (no need for radix complement since both negative)
        sum.coefficient = add_coefficients(int_a.coefficient, int_b.coefficient);

        // Change the sign of sum big int to negative
        sum.integer_sign = sign::NEGATIVE;
    }

    // first int is negative and 2nd is positive
    else if (int_a.get_sign() == sign::NEGATIVE && int_b.get_sign() == sign::POSITIVE)
    {
        // add radix of a and b coefficient
        sum.coefficient = add_coefficients(int_a.radix_complement(), int_b.coefficient);

        /* update sum big int sign to negative if int_a is a bigger value
           and update the sum digits to the radix complement (since result is negative and in radix complement)*/
        if (int_a.coefficient_size() > int_b.coefficient_size())
        {
            sum.integer_sign = sign::NEGATIVE;
            sum.coefficient = sum.radix_complement();
        }
        else if (int_a.coefficient_size() == int_b.coefficient_size())
        {
            uint64_t size = int_a.coefficient_size();
            // Loop through each digit until find one that is bigger than the other, indicating which int is bigger overall
            for (uint64_t index = size; index > 0; index--)
            {
                // set sign to negative and set digits to the radix complement
                if (int_a.coefficient[index - 1] > int_b.coefficient[index - 1])
                {
                    sum.integer_sign = sign::NEGATIVE;
                    sum.coefficient = sum.radix_complement();
                    break;
                }
                // remove the extra carry digit of 1 if a < b, due to radix complement
                else if (int_a.coefficient[index - 1] < int_b.coefficient[index - 1])
                {
                    sum.shrink();
                    sum.coefficient[sum.coefficient_size() - 1] = sum.coefficient[sum.coefficient_size() - 1] - 1;
                    sum.shrink();
                    break;
                }
            }
        }
        // need to remove extra leading digit added by using radix complement since negative int is smaller
        else
        {
            sum.shrink();
            sum.coefficient[sum.coefficient_size() - 1] = sum.coefficient[sum.coefficient_size() - 1] - 1;
            sum.shrink();
        }
    }
    // One int is positive and other is negative
    else if (int_a.get_sign() == sign::POSITIVE && int_b.get_sign() == sign ::NEGATIVE)
    {
        // add a coefficient and radix of b
        sum.coefficient = add_coefficients(int_a.coefficient, int_b.radix_complement());

        /* update sum big int sign to negative if int_b is a bigger value
           and update the sum digits to the radix complement (since result is negative and in radix complement)*/
        if (int_a.coefficient_size() < int_b.coefficient_size())
        {
            sum.integer_sign = sign::NEGATIVE;
            sum.coefficient = sum.radix_complement();
        }
        else if (int_a.coefficient_size() == int_b.coefficient_size())
        {
            uint64_t size = int_a.coefficient_size();
            // Loop through each digit until find one that is bigger than the other, indicating which int is bigger overall
            for (uint64_t index = size; index > 0; index--)
            {
                // set sign to negative and set digits to the radix complement
                if (int_a.coefficient[index - 1] < int_b.coefficient[index - 1])
                {
                    sum.integer_sign = sign::NEGATIVE;
                    sum.coefficient = sum.radix_complement();
                    break;
                }
                // remove the extra carry digit of 1 if a < b, due to radix complement
                else if (int_a.coefficient[index - 1] > int_b.coefficient[index - 1])
                {
                    sum.shrink();
                    sum.coefficient[sum.coefficient_size() - 1] = sum.coefficient[sum.coefficient_size() - 1] - 1;
                    sum.shrink();
                    break;
                }
            }
        }
        // need to remove extra leading digit added by using radix complement since negative int is smaller
        else
        {
            sum.shrink();
            sum.coefficient[sum.coefficient_size() - 1] = sum.coefficient[sum.coefficient_size() - 1] - 1;
            sum.shrink();
        }
    }
    // both integers are positive case
    else
    {
        // add the coefficients of both a and b, no need to change sign of sum
        sum.coefficient = add_coefficients(int_a.coefficient, int_b.coefficient);
    }

    return sum;
}

big_int operator-(const big_int &int_a, const big_int &int_b)
{
    return int_a + (-int_b);
}

big_int operator*(const big_int &int_a, const big_int &int_b)
{
    big_int product;

    // multiply each coefficient/digit of the multiplier (int_b) by each coefficient/digit of the multiplicand (int_a)
    for (uint64_t i = 0; i < int_a.coefficient_size(); i++)
    {
        uint64_t carry = 0;
        // multiply digit of int_a by each digit of int_b and add to the product big_int at index (i+j) if it exists
        for (uint64_t j = 0; j < int_b.coefficient_size() && (i + j) < product.coefficient_size(); j++)
        {
            uint64_t temp = (int_a.coefficient[i] * (uint64_t)int_b.coefficient[j]) + carry + product.coefficient[i + j];
            product.coefficient[i + j] = (uint32_t)(temp & (uint64_t)(UINT32_MAX));
            carry = temp >> 32;
        }
        // if the product big int coefficient vector is not long enough start adding new coefficients with the value product + carry
        for (uint64_t k = product.coefficient_size(); k < int_b.coefficient_size(); k++)
        {
            uint64_t temp = (int_a.coefficient[i] * (uint64_t)int_b.coefficient[k]) + carry;
            product.coefficient.push_back((uint32_t)(temp & (uint64_t)(UINT32_MAX)));
            carry = temp >> 32;
        }

        //add new coefficient with carry if any
        if (carry != 0)
        {
            product.coefficient.push_back((uint32_t)carry);
        }
    }

    // update the sign of the product big_int if only one of the two integers multiplied are negative
    if (int_a.get_sign() == sign::NEGATIVE && int_b.get_sign() == sign::POSITIVE)
    {
        product.integer_sign = sign::NEGATIVE;
    }
    else if (int_a.get_sign() == sign::POSITIVE && int_b.get_sign() == sign ::NEGATIVE)
    {
        product.integer_sign = sign::NEGATIVE;
    }

    return product;
}

big_int operator/(const big_int &dividend, const big_int &divisor)
{
    // check for division by zero and throw exception
    if (divisor.at(0) == 0 && divisor.coefficient_size() == 1)
    {
        throw division_by_zero();
    }

    big_int quotient;
    big_int one(1);
    uint64_t dividend_size = dividend.coefficient_size();
    uint64_t divisor_size = divisor.coefficient_size();
    // If dividend smaller return zero
    if (dividend_size < divisor_size)
    {
        return quotient;
    }
    // if divisor equals 1 ***Optimization TODO***
    if (divisor == one)
    {
        quotient = dividend;
        return quotient;
    }

    // set the remainder big_int to the 1st divisor_size - 1 digits of the dividend
    big_int remainder;
    remainder.coefficient.erase(remainder.coefficient.begin());
    for (uint64_t index = dividend_size - 1 - (divisor_size - 2); index < dividend_size; index++)
    {
        remainder.coefficient.push_back(dividend.coefficient[index]);
    }

    //Long Division as if two positive integers
    for (uint64_t i = 0; i < dividend_size - divisor_size + 1; i++)
    {
        //Add the next digit of the dividend as the least significant digit of the remainder
        remainder.coefficient.insert(remainder.coefficient.begin(), dividend.coefficient[dividend_size - divisor_size - i]);

        //Find the number to times the divisor can got into the remainder value (the count)
        uint32_t count = 0;
        // Account for negative divisor and the subtraction
        if (divisor.get_sign() == sign::NEGATIVE)
        {
            big_int sub = remainder + divisor;
            while (sub.get_sign() == sign::POSITIVE)
            {
                count++;
                sub = sub + divisor;
            }

            remainder = sub - divisor;
        }
        // if divisor is positive, then do regular long division
        else
        {
            big_int sub = remainder - divisor;
            while (sub.get_sign() == sign::POSITIVE)
            {
                count++;
                sub = sub - divisor;
            }

            // Set remainder to be previous remainder - dividend*count
            remainder = sub + divisor;
        }

        // Add the count as the next least significant digit of the quotient
        quotient.coefficient.insert(quotient.coefficient.begin(), count);
    }

    // Update the sign of the quotient either divisor or divident is negative
    if (dividend.get_sign() == sign::NEGATIVE && divisor.get_sign() == sign::POSITIVE)
    {
        quotient.integer_sign = sign::NEGATIVE;
    }
    else if (dividend.get_sign() == sign::POSITIVE && divisor.get_sign() == sign::NEGATIVE)
    {
        quotient.integer_sign = sign::NEGATIVE;
    }

    return quotient;
}

// Referenced: https://www.cplusplus.com/reference/algorithm/reverse/
string print_base10(const big_int &integer)
{
    stringstream base10;
    big_int copy(integer);

    // Divide the big integer by 10 and the remainder is a digit of  the integer in base 10, until big integer is zero
    do
    {
        base10 << copy.remainder_32(10);
        copy.divide_32(10);
        copy.shrink(); // remove extra zeros
    } while (copy.at(0) != 0);

    // add sign of big integer to string
    if (copy.get_sign() == sign::POSITIVE)
    {
        base10 << '+';
    }
    else
    {
        base10 << '-';
    }

    // Referse the integer string since remainder starts producing from least significant digit
    string int_reverse = base10.str();
    reverse(int_reverse.begin(), int_reverse.end());

    return int_reverse;
}

/************************** Helper Functions and other Operator Overloads *************************/
big_int operator-(const big_int &int_a)
{
    // Copy int a and use negate member function on the copy
    big_int negation(int_a);
    negation.negate();
    return negation;
}

bool operator<(const big_int &int_a, const big_int &int_b)
{
    sign a_sign = int_a.get_sign();
    sign b_sign = int_b.get_sign();
    uint64_t a_size = int_a.coefficient_size();
    uint64_t b_size = int_b.coefficient_size();

    // Check if one int is negative and the other is positive
    if (a_sign == sign::NEGATIVE && b_sign == sign::POSITIVE)
        return true;
    else if (int_a.get_sign() == sign::POSITIVE && int_b.get_sign() == sign::NEGATIVE)
        return false;

    // Since both ints have same sign, check if int a has more digits than b and return based on sign
    else if (a_size < b_size)
    {
        if (a_sign == sign::POSITIVE)
            return true;
        else
            return false;
    }
    else if (a_size > b_size)
    {
        if (a_sign == sign::POSITIVE)
            return false;
        else
            return true;
    }

    // Both ints have same number of digits, starting with most significant, go digit by digit and compare
    // If digits are the same move to next digit
    bool flag = false;
    bool same_value = true;
    for (uint64_t i = a_size; i > 0; i--)
    {
        // Check if digit of a is < digit of b
        if (int_a.at(i - 1) < int_b.at(i - 1))
        {
            flag = true;
            same_value = false;
            break;
        }
        // Check if digit of a > digit of b
        else if (int_a.at(i - 1) > int_b.at(i - 1))
        {
            flag = false;
            same_value = false;
            break;
        }
    }

    // Check if both are negative
    if (int_a.get_sign() == sign::NEGATIVE && int_b.get_sign() == sign::NEGATIVE && !same_value)
    {
        return !flag;
    }
    // Check if same value (both are positive)
    else if (!same_value)
    {
        return flag;
    }
    return false;
}

bool operator>(const big_int &int_a, const big_int &int_b)
{
    return int_b < int_a;
}

bool operator!=(const big_int &int_a, const big_int &int_b)
{
    // Check if each digit of both integers are the same value
    for (uint64_t index = 0; index < int_a.coefficient_size(); index++)
    {
        if (int_a.at(index) != int_b.at(index))
            return true;
    }

    // Since both ints are the same check for different signs
    if (int_a.get_sign() != int_b.get_sign())
        return true;

    return false;
}

bool operator==(const big_int &int_a, const big_int &int_b)
{
    return !(int_a != int_b);
}

vector<uint32_t> add_coefficients(const vector<uint32_t> &vec1, const vector<uint32_t> &vec2)
{
    vector<uint32_t> sum;
    uint64_t vec1_size = vec1.size();
    uint64_t vec2_size = vec2.size();
    uint64_t carry = 0;
    uint64_t temp = 0;

    //For the rest of the coefficients add them and then carry (until the end of smallest coefficient vector)
    for (uint64_t i = 0; i < vec1_size && i < vec2_size; i++)
    {
        temp = (vec1[i] + (uint64_t)vec2[i]) + carry;
        sum.push_back((uint32_t)(temp & (uint64_t)UINT32_MAX));
        carry = temp >> 32;
    }

    // if the coefficient vectors of int_a and int_b are not the same, add the extra coefficients of bigger big_int plus the carry to the new big int
    if (vec1_size < vec2_size)
    {
        for (uint64_t i = vec1_size; i < vec2_size; i++)
        {
            temp = vec2[i] + carry;
            sum.push_back((uint32_t)(temp & (uint64_t)UINT32_MAX));
            carry = temp >> 32;
        }
    }
    else if (vec2_size < vec1_size)
    {
        for (uint64_t i = vec2_size; i < vec1_size; i++)
        {
            temp = vec1[i] + carry;
            sum.push_back((uint32_t)(temp & (uint64_t)UINT32_MAX));
            carry = temp >> 32;
        }
    }

    // Check to see if there is a carry left over
    if (carry != 0)
    {
        sum.push_back((uint32_t)carry);
    }

    return sum;
}
