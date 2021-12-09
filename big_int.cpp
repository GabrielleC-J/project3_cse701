/**
 * @file BigInt.cpp
 * @author Gabrielle Ching-Johnson
 * @brief
 * @version 0.1
 * @date Dec 8, 2021
 */

#include <iostream>
#include <vector>
#include <cstdint>
#include "big_int.hpp"
using namespace std;

/******************************* Constructors ******************************/
big_int::big_int() : coefficient({0}){};

big_int::big_int(const uint64_t &size) : coefficient((size)){};

big_int::big_int(const int64_t &integer)
{
    // Add the coefficients/digits for the integer in base 2^32
    uint64_t value = integer;
    uint64_t remainder = value % base;
    coefficient.push_back(remainder);
    while (value > base)
    {
        value = value / base;
        remainder = value % base;
        coefficient.push_back(remainder);
    }

    // Find out if integer is positive or negative
    if (integer < 0)
    {
        integer_sign = sign::NEGATIVE;

        // set the integer to its radix complement
        uint64_t radix_minus1 = base - 1;
        // Find radix - 1 complement and then add 1 to get radix complement
        for (uint64_t &digit : coefficient)
        {
            digit = radix_minus1 - digit;
        }
        // Add 1 to the big int and check carry
        uint64_t carry = 0;
        for (uint64_t &digit : coefficient)
        {
            uint64_t temp = digit + 1 + carry;
            digit = temp & UINT32_MAX; // keep the low 32 bits
            carry = temp >> 32;        // shift temp to the right by 32 bits to get the carry bit
        }

        // If the carry is not 0, need to add a new coefficient with the value of carry
        if (carry != 0)
        {
            coefficient.push_back(carry);
        }
    }
}

big_int::big_int(const string &integer)
{
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

/******************************* Public Functions ******************************/
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