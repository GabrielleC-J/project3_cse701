/**
 * @file BigInt.cpp
 * @author Gabrielle Ching-Johnson
 * @brief
 * @version 0.1
 * @date Dec 9, 2021
 */

#include <iostream>
#include <vector>
#include <cstdint>
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
    uint64_t remainder = value % base;
    coefficient.push_back(remainder);
    while (value > base)
    {
        value = value / base;
        remainder = value % base;
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

    // Check to make sure all the characters are digits from 1 to 9 **TODO

    // For each digit character in the string multiply the big integer by 10 and then add the digit value
    for (const char &digit : integer_temp)
    {
        multiply(10);
        add_32(digit - '0'); // the character digit will have ascii value thus this value minus the value at 0 will give base 10 digit value
    }
}

big_int::big_int(const big_int &big_integer)
{
    for (const uint64_t &digits : big_integer.coefficient) // Don't think this is allowed because object is accessing private var
    {
        coefficient.push_back(digits);
    }
}

big_int::big_int(const vector<uint32_t> &vec)
{
    coefficient.reserve(vec.size());
    for (uint64_t i = vec.size() - 1; i > 0; i--)
    {
        coefficient.push_back(vec[i]);
    }
    coefficient.push_back(vec[0]);
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

const uint64_t &big_int::at(const uint64_t &index) const
{
    return coefficient.at(index);
}

uint64_t big_int::coefficient_size() const
{
    //First if there are more than 1 coefficient need to shrink, in case there are extra 0 coefficients **TODO**
    return coefficient.size();
}

const sign &big_int::get_sign() const
{
    return integer_sign;
}
/******************************* Private Functions ******************************/
void big_int::multiply(const uint32_t &integer)
{
    uint64_t carry = 0;
    for (uint64_t &digit : coefficient)
    {
        uint64_t temp = (digit * integer) + carry;
        digit = temp & (uint64_t)UINT32_MAX; //Keep the lower 32 bits as the value for the coefficient digit
        carry = temp >> 32;                  // Assign value of the carry by shifting temp value to the right by 32;
    }

    // Check to see if carry at the end is 0, if not add new coefficient to vector of coefficients
    if (carry != 0)
    {
        coefficient.push_back(carry);
    }
}

void big_int::add_32(const uint32_t &integer)
{
    // Add integer to only the very first coefficient (since the integer is only 32 bits)
    uint64_t temp = coefficient[0] + integer;
    coefficient[0] = temp & (uint64_t)UINT32_MAX; // keep the low 32 bits
    uint64_t carry = temp >> 32;                  // shift temp to the right by 32 bits to get the carry bit

    // Check if a carry exists and update next coefficient until carry = 0 or until the end of the coefficient vector
    for (uint64_t i = 1; i < coefficient.size() && carry != 0; i++)
    {
        uint64_t temp = coefficient[i] + carry;
        coefficient[i] = temp & (uint64_t)UINT32_MAX; // keep the low 32 bits
        carry = temp >> 32;                           // shift temp to the right by 32 bits to get the carry bit
    }

    // If the carry is not 0, need to add a new coefficient with the value of carry
    if (carry != 0)
    {
        coefficient.push_back(carry);
    }
}

vector<uint64_t> big_int::radix_complement() const
{
    vector<uint64_t> complement;
    // Ged the radix - 1
    uint64_t radix_minus1 = base - 1;

    // Find radix - 1 complement and then add 1 to get radix complement
    for (const uint64_t &digit : coefficient)
    {
        complement.push_back(radix_minus1 - digit);
    }

    // Add 1 to the big int and check carry
    uint64_t temp = complement[0] + 1;
    complement[0] = temp & (uint64_t)UINT32_MAX; // keep the low 32 bits
    uint64_t carry = temp >> 32;

    // If a carry exists add it to the next element in vector if it exist
    for (uint64_t i = 1; i < complement.size() && carry != 0; i++)
    {
        uint64_t temp = complement[i] + carry;
        complement[i] = temp & (uint64_t)UINT32_MAX; // keep the low 32 bits
        carry = temp >> 32;                          // shift temp to the right by 32 bits to get the carry bit
    }

    // If the carry is not 0, need to add a new element with the value of carry
    if (carry != 0)
    {
        complement.push_back(carry);
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

    if (coefficient.size() > 1)
    {
        bool number_reached = false;
        for (int i = coefficient.size() - 1; i > 0 && !number_reached; i--)
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
    if (int_a.get_sign() == sign::NEGATIVE && int_b.get_sign() == sign ::NEGATIVE)
    {
        // add the two radix complements
        sum.coefficient = add_coefficients(int_a.radix_complement(), int_b.radix_complement());

        // Change the sign of sum big int to negative
        sum.integer_sign == sign::NEGATIVE;
    }
    else if (int_a.get_sign() == sign::NEGATIVE && int_b.get_sign() == sign::POSITIVE)
    {
        // add radix of a and b coefficient
        sum.coefficient = add_coefficients(int_a.radix_complement(), int_b.coefficient);

        // update sum big int sign to negative if int_a is a bigger value
        if (int_a.coefficient_size() > int_b.coefficient_size())
        {
            sum.integer_sign = sign::NEGATIVE;
        }
        else if (int_a.coefficient_size() == int_b.coefficient_size())
        {
            uint64_t size = int_a.coefficient_size();
            if (int_a.coefficient[size] > int_b.coefficient[size])
            {
                sum.integer_sign = sign::NEGATIVE;
            }
        }
    }
    else if (int_a.get_sign() == sign::POSITIVE && int_b.get_sign() == sign ::NEGATIVE)
    {
        // add a coefficient and radix of b
        sum.coefficient = add_coefficients(int_a.coefficient, int_b.radix_complement());

        // update sum big int sign to negative if int_b is a bigger value
        if (int_a.coefficient_size() < int_b.coefficient_size())
        {
            sum.integer_sign = sign::NEGATIVE;
        }
        else if (int_a.coefficient_size() == int_b.coefficient_size())
        {
            uint64_t size = int_a.coefficient_size();
            if (int_a.coefficient[size] < int_b.coefficient[size])
            {
                sum.integer_sign = sign::NEGATIVE;
            }
        }
    }
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

    // multiply each coefficient/digit of the multiplier (int_b) by each coefficient of the multiplicand (int_a)
    for (uint64_t i = 0; i < int_a.coefficient_size(); i++)
    {
        uint64_t carry = 0;
        // multiply digit of int_a by each digit of int_b and add to the product big_int at index (i+j) if it exists
        for (uint64_t j = 0; j < int_b.coefficient_size() && (i + j) < product.coefficient_size(); j++)
        {
            uint64_t temp = (int_a.coefficient[i] * int_b.coefficient[j]) + carry + product.coefficient[i + j];
            product.coefficient[i + j] = temp & (uint64_t)(UINT32_MAX);
            carry = temp >> 32;
        }
        // if the product big int coefficient vector is not long enough start adding new coefficients with the value product + carry
        for (uint64_t k = product.coefficient_size(); k < int_b.coefficient_size(); k++)
        {
            uint64_t temp = (int_a.coefficient[i] * int_b.coefficient[k]) + carry;
            product.coefficient.push_back(temp & (uint64_t)(UINT32_MAX));
            carry = temp >> 32;
        }

        //add new coefficient with carry if any
        if (carry != 0)
        {
            product.coefficient.push_back(carry);
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

/************************** Helper Functions and other Operator Overloads *************************/
big_int operator-(const big_int &int_a)
{
    big_int negation(int_a);
    negation.negate();
    return negation;
}

vector<uint64_t> add_coefficients(const vector<uint64_t> &vec1, const vector<uint64_t> &vec2)
{
    vector<uint64_t> sum;
    uint64_t vec1_size = vec1.size();
    uint64_t vec2_size = vec2.size();
    uint64_t carry = 0;
    uint64_t temp = 0;

    //For the rest of the coefficients add them and then carry (until the end of smallest coefficient vector)
    for (uint64_t i = 0; i < vec1_size || i < vec2_size; i++)
    {
        temp = vec1[i] + vec2[i] + carry;
        sum.push_back(temp & (uint64_t)UINT32_MAX);
        carry = temp >> 32;
    }

    // if the coefficient vectors of int_a and int_b are not the same, add the extra coefficients plus the carry to the new big int
    if (vec1_size < vec2_size)
    {
        for (uint64_t i = vec1_size; i < vec2_size; i++)
        {
            temp = vec2[i] + carry;
            sum.push_back(temp & (uint64_t)UINT32_MAX);
            carry = temp >> 32;
        }
    }
    else if (vec2_size < vec1_size)
    {
        for (uint64_t i = vec2_size; i < vec1_size; i++)
        {
            temp = vec1[i] + carry;
            sum.push_back(temp & (uint64_t)UINT32_MAX);
            carry = temp >> 32;
        }
    }

    // Check to see if there is a carry left over
    if (carry != 0)
    {
        sum.push_back(carry);
    }

    return sum;
}

/* big_int::add(const big_int &num)
{
    //Check to make sure the big integers have the same number of coefficients

    // Add each coefficient and include carry

    uint64_t carry = 0;

    for (uint64_t i = 0; i < coefficient.size(); i++)
    {
        uint64_t sum = coefficient[i] + num.at(i) + carry;
        carry = sum >> 32;
        uint64_t max = UINT32_MAX;
        result.at(i) = sum & max;
    }

    return result;
}
 */
/**************************** Operator Overloads **********************/

/* ostream &operator<<(ostream &out, const BigInt &num)
{
    out << '(';
    for (uint64_t i = num.coefficient_size() - 1; i >= 0; i--)
    {
        out << num.coefficient[i] << ",";
    }
} */