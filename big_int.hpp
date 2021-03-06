#pragma once
/**
 * @file big_int.hpp
 * @author Gabrielle Ching-Johnson
 * @brief Header file for arbitrary precision integer arithmetic through the creation of a class called big_int that represents big integers
 * @version 0.2
 * @date Dec 19, 2021
 */

#include <iostream>
#include <vector>
#include <string>
#include <cstdint>
using namespace std;

/**
 * @brief This enum class will be used to indicate whetHer a big_int is positive or negative
 */
enum class sign
{
    POSITIVE,
    NEGATIVE
};

/**
 * @brief This class represents a big integer that can hold integers bigger than the maximum 64 bits and can do arbitrary precision integer arithmetic
 */
class big_int
{
    /*************************** Friend Functions ************************/
    /**
     * @brief A friend function that adds two big integers
     * @param int_a The first big_int to be added
     * @param int_b The second big_int to be added
     * @return A big_int that represents the sum of the two big_ints added
     */
    friend big_int operator+(const big_int &int_a, const big_int &int_b);

    /**
     * @brief A friend function that subtracts two big integers
     * @param int_a The first big_int to be subtracted from
     * @param int_b The second big_int to subtract from the first
     * @return A big_int that represents the result of the subtraction of the two big_ints
     */
    friend big_int operator-(const big_int &int_a, const big_int &int_b);

    /**
     * @brief A friend function that multiplies two big integers
     * @param int_a The first big_int to be multiplied
     * @param int_b The second big_int to be multiplied
     * @return A big_int that represents the result of the two big_ints multiplied
     */
    friend big_int operator*(const big_int &int_a, const big_int &int_b);

    /**
     * @brief A friend function that divides two big integers
     * @param int_a The big_int that is the dividend
     * @param int_b The big_int that is the divisor
     * @return A big_int that represents the result of the two big_ints divided
     */
    friend big_int operator/(const big_int &dividend, const big_int &divisor);

    /**
     * @brief Returns the string base 10 value of the big integer
     * @param integer A big_int that will be converted to base 10
     * @return A string containing the base 10 value of the big integer
     */
    friend string print_base10(const big_int &integer);

public:
    /************************** Constructors ****************************/
    /**
     * @brief Initialize a big integer with value 0
     */
    big_int();

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
     * @brief Copy Constructor -> Initialize a big integer (big_int) from another big integer
     * @param big_integer A big_int to copy
     */
    big_int(const big_int &big_integer);

    /******************* Public Member Functions *******************/
    /**
     * @brief The negation operation of the current big integer
     */
    void negate();

    /**
     * @brief Checks if index is in range and gets coefficient at index
     * @details Retrieves the coefficient at specified index and throws out_of_range error
     *          via vector vector::at()
     * @param index the index to find the coefficient at
     */
    const uint32_t &at(const uint64_t &index) const;

    /**
     * @brief Gets the number of coefficients in the big integer
     * @return an integer indicating the number of coefficients in the big integer
     */
    uint64_t coefficient_size() const;

    /**
     * @brief Retrieves the sign of the integer, either positive or negative (zero will have a positive sign)
     * @return A sign object indicating the sign
     */
    const sign &get_sign() const;

    /**
     * @brief Operator overload for the =
     * @param big_integer A big integer to be copied
     * @return A reference to a new big_int that is a copy of the argument
     */
    big_int &operator=(const big_int &big_integer);

private:
    // List containing all coefficients for the big number starting with least significant at index 0
    vector<uint32_t> coefficient;

    // Type sign that indications the sign of the integer (positive or negative)
    sign integer_sign = sign::POSITIVE;

    // Base value of the numbers stored in the vector, instead of base 10 it will be base 2^32
    uint64_t base = (uint64_t)UINT32_MAX + 1; //4294967296;

    /**
     * @brief Increases the number of coefficients for the Big Integers by adding leading zeros
     * @param num_zeros The number of leading zer0 coefficients to add
     */
    void expand(uint64_t const &num_zeros);

    /**
     * @brief Removes all leading 0 coefficients for the big number
     */
    void shrink();

    /**
     * @brief Multiplies an unsigned 32 bit integer to the current big number
     * @details Helper function for reading in a string
     * @param integer a 32 bit integer
     */
    void multiply_32(const uint32_t &integer);

    /**
     * @brief Adds an unsigned 32 bit integer to the current big number
     * @details Helper function for reading in string
     * @param integer a 32 bit integer
     */
    void add_32(const uint32_t &integer);

    /**
     * @brief Divides the current big number by an unsigned 32 bit integer
     * @details Helper function for writing to a string
     * @param integer a 32 bit integer
     */
    void divide_32(const uint32_t &integer);

    /**
     * @brief Finds the remainder of the current big int divided by an unsigned 32 bit integer
     * @details Helper function for writing to a string
     * @param integer a 32 bit integer
     * @return integer indicating the remainder
     */
    uint32_t remainder_32(const uint32_t &integer) const;

    /**
     * @brief Finds the radix complement of the big integer where the radix is 2^32
     * @details This will be used for addition
     * @return a vector of unsigned 32 bit integers representing the radix complement of the big int
     */
    vector<uint32_t> radix_complement() const;
};

/***************************** Exceptions ***************************/
/**
 * @brief Exception to be thrown if the base 10 integer input string contains a character that is not a number or a +/- in the beginning
 */
class invalid_string_integer : public invalid_argument
{
public:
    invalid_string_integer() : invalid_argument("Invalid character found. The integer string inputted for the big_int can not have any other characters but numbers and a + or - sign in the beginning"){};
};

/**
 * @brief Exception to be thrown if dividing by zero
 */
class division_by_zero : public invalid_argument
{
public:
    division_by_zero() : invalid_argument("Cannot divide a big integer by zero"){};
};

/************************** Helper Functions and Other Operator Overloads *************************/
/**
 * @brief Negation operator overload 
 * @details Finds the negation of the argument big int and returns a new big int with the negation value
 * @param int_a The big_int to be negated
 * @return A big_int that represents the result 
 */
big_int operator-(const big_int &int_a);

/**
 * @brief Operator overload for <
 * @param int_a the first big_int to compare
 * @param int_b the second big_int to compare
 * @return A boolean value stating if int_a < int_b
 */
bool operator<(const big_int &int_a, const big_int &int_b);

/**
 * @brief Operator overload for >
 * @param int_a the first big_int to compare
 * @param int_b the second big_int to compare
 * @return A boolean value stating if int_a > int_b
 */
bool operator>(const big_int &int_a, const big_int &int_b);

/**
 * @brief Operator overload for !=
 * @param int_a the first big_int to compare
 * @param int_b the second big_int to compare
 * @return A boolean value stating if int_a != int_b
 */
bool operator!=(const big_int &int_a, const big_int &int_b);

/**
 * @brief Operator overload for ==
 * @param int_a the first big_int to compare
 * @param int_b the second big_int to compare
 * @return A boolean value stating if int_a == int_b
 */
bool operator==(const big_int &int_a, const big_int &int_b);

/**
 * @brief Adds two vectors element by element and incorporates the carry as if adding two big integers
 * @details Helper function function for the + operator overload
 * @param vec1 Vector of coefficients
 * @param vec2 Vector of coefficients
 * @return a vector of 64 bit integers representing the sum of the two vectors
 */
vector<uint32_t> add_coefficients(const vector<uint32_t> &vec1, const vector<uint32_t> &vec2);
