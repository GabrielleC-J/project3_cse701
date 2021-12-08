/**
 * @file main.cpp
 * @author Gabrielle Ching-Johnson
 * @brief
 * @version 0.1
 * @date Dec 5, 2021
 */

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <filesystem>
#include <sstream>
#include "big_int.hpp"
using namespace std;

uint8_t passed_counter = 0;
uint8_t failed_counter = 0;

void check(bool condition)
{
    if (condition)
    {
        //Print "-> Passed"
    }

    else
    {
        //Print to file "-> Failed"
    }
}

int main()
{
}