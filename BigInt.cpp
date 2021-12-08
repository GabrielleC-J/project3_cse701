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
#include "BigInt.hpp"
using namespace std;

BigInt::BigInt() : coefficient({0}){};

BigInt::BigInt(const uint64_t &size) : coefficient((size)){};

BigInt::BigInt(const vector<uint32_t> &vec)
{
    coefficient.reserve(vec.size());
    for (uint64_t i = vec.size() - 1; i > 0; i--)
    {
        coefficient.push_back(vec[i]);
    }
    coefficient.push_back(vec[0]);
}

uint64_t BigInt::coefficient_size() const
{
    return coefficient.size();
}

const uint64_t &BigInt::at(const uint64_t &index) const
{
    return coefficient.at(index);
}

uint64_t &BigInt::at(const uint64_t &index)
{
    return coefficient.at(index);
}

BigInt BigInt::add(const BigInt &num)
{
    //Check to make sure the big integers have the same number of coefficients

    // Add each coefficient and include carry
    BigInt result = BigInt(coefficient.size());
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

/**************************** Operator Overloads **********************/

/* ostream &operator<<(ostream &out, const BigInt &num)
{
    out << '(';
    for (uint64_t i = num.coefficient_size() - 1; i >= 0; i--)
    {
        out << num.coefficient[i] << ",";
    }
} */