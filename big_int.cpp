/**
 * @file BigInt.cpp
 * @author Gabrielle Ching-Johnson
 * @brief
 * @version 0.1
 * @date Dec 5, 2021
 */

#include <iostream>
#include <vector>
#include <cstdint>
#include "big_int.hpp"
using namespace std;

big_int::big_int() : coefficient({0}){};

big_int::big_int(const uint64_t &size) : coefficient((size)){};

big_int::big_int(const vector<uint32_t> &vec)
{
    coefficient.reserve(vec.size());
    for (uint64_t i = vec.size() - 1; i > 0; i--)
    {
        coefficient.push_back(vec[i]);
    }
    coefficient.push_back(vec[0]);
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