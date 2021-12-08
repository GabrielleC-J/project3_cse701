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
     * @brief Initialize a big integer with value 0
     * @param size the number of coefficients for the Big Integer
     */
    BigInt(const uint64_t &size);

    /**
     * @brief Initialize a big integer from a vector of values
     * @details These values would be 32 bit integers where the 1st integer represents the most significant integer of the big Integer
     * @param vec A vector of 32 bit integers
     */
    BigInt(const vector<uint32_t> &vec);

    /**
     * @brief Gets the number of coefficients in the big integer
     * @return an integer indicating the number of coefficients in the big integer
     */
    uint64_t coefficient_size() const;

    /**
     * @brief Checks if index is in range and gets coefficient at index
     * @details Retrieves the coefficient at specified index and throws out_of_range error
     *          via vector vector::at()
     * @param index the index to find the coefficient at
     */
    const uint64_t &at(const uint64_t &index) const;

    /**
     * @brief Checks if index is in range and gets coefficient at index
     * @details Retrieves the coefficient at specified index and throws out_of_range error
     *          via vector vector::at(). Allows for modification of value.
     * @param index the index to find the coefficient at
     */
    uint64_t &at(const uint64_t &index);

    /**
     * @brief Adds another Big integer to this Big interger
     * @param num A big integer that is to be added
     * @return A Big integer that is the sum of the two Big integers
     */
    BigInt add(const BigInt &num);

    /**
     * @brief Subtracts another Big integer from this Big integer
     * @param num A big integer that is to be added
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

/************************* Operator Overloads ***********************/

/**
 * @brief Operator overload for <<
 * @return an ostream
 */
ostream &operator<<(ostream &out, const BigInt &num);
