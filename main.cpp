/**
 * @file main.cpp
 * @author Gabrielle Ching-Johnson
 * @brief
 * @version 0.1
 * @date Dec 5, 2021
 */

#include <iostream>
#include <vector>
#include "BigInt.hpp"
using namespace std;

int main()
{
    uint32_t character = (int)'c';
    vector<uint32_t> testing = {character, (int)'1', (int)'2'};
    vector<uint32_t> testing2 = {(int)'2', (int)'2'};
    vector<uint32_t> testing1 = {(int)'1', (int)'1'};
    BigInt zero = BigInt();
    BigInt test = BigInt(testing);
    BigInt one = BigInt(testing1);
    BigInt two = BigInt(testing2);
    BigInt result = one.add(two);
}