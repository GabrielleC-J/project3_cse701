#pragma once
/**
 * @file BigInt.hpp
 * @author Gabrielle Ching-Johnson
 * @brief
 * @version 0.1
 * @date Dec 5, 2021
 */

#include <iostream>
#include <vector>
using namespace std;

class BigInt
{
public:
    /**
     * @brief Initialize a big integer with value 0
     */
    BigInt();

    /**
     * @brief Initialize a big integer from a vector of values
     * @details These values would be 32 bit integers where the 1st integer represents the most significant integer of the big Integer
     * @param vec A vector of 32 bit integers
     */
    BigInt(vector<uint32_t> const &vec);

    /**
     * @brief Adds another Big integer to this Big interger
     * @return A Big integer that is the sum of the two Big integers
     */
    BigInt add(BigInt &num);

    /**
     * @brief Subtracts another Big integer from this Big integer
     * @return A Big integer that is the result of the subtraction
     */
    BigInt sub(BigInt &num);

private:
    // List containing all coefficients for the big number starting with least significant at index 0
    vector<uint64_t> coefficient;

    /**
     * @brief Increases the number of coefficients for the Big Integers by adding leading zeros
     * @param num_zeros The number of leading zeros to add
     */
    void expand(uint32_t const &num_zeros);

    /**
     * @brief Removes all leading 0 coefficients for the big number
     */
    void shrink();
};
