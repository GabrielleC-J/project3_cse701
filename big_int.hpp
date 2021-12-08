#pragma once
/**
 * @file big_int.hpp
 * @author Gabrielle Ching-Johnson
 * @brief Header file for arbitrary precision integer arithmetic through the creation of a class called big_int that represents big integers
 * @version 0.1
 * @date Dec 8, 2021
 */

#include <iostream>
#include <vector>
#include <string>
using namespace std;

/**
 * @brief This class represents a big integer that can hold integers bigger than the maximum 64 bits and can do arbitrary precision integer arithmetic
 */
class big_int
{

public:
    /************************** Constructors ****************************/
    /**
     * @brief Initialize a big integer with value 0
     */
    big_int();

    /**
     * @brief Initialize a big integer with value 0
     * @param size the number of coefficients for the Big Integer
     */
    big_int(const uint64_t &size);

    /**
     * @brief Initialize a big integer from a 64 bit decimal integer (ie base 10)
     * @param integer a signed 64 bit decimal integer
     */
    big_int(const int64_t &integer);

    /**
     * @brief Initialize a big integer from a string containing a base 10 integer
     * @details The integer in the string can have a "+" or "-" in the beginning of it or neither to indicate a positive integer
     * @param integer a string containing an integer
     */
    big_int(const string &integer);

    /**
     * @brief Initialize a big integer from a vector of values
     * @details These values would be 32 bit integers where the 1st integer represents the most significant integer of the big Integer
     * @param vec A vector of 32 bit integers
     */
    big_int(const vector<uint32_t> &vec);

    /**
     * @brief Copy Constructor -> Initialize a big integer(big_int) from another big integer
     * @param big_integer A big_int to copy
     */
    big_int(const big_int &big_integer);

    /******************* Public Member Functions *******************/
    /**
     * @brief Adds another big integer to this big interger
     * @param num A big_int that is to be added
     */
    void add(const big_int &num);

    /**
     * @brief Subtracts another big integer from this Big integer
     * @param num A big_int that is to be added
     */
    void sub(big_int &num);

    /**
     * @brief The negation operation of the current big integer
     */
    void negate();

    /**
     * @brief Returns the string base 10 value of the big integer
     * @return A string containing the base 10 value of the big integer
     */
    string print_decimal() const;

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
ostream &operator<<(ostream &out, const big_int &num);