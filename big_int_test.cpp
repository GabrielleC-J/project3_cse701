/**
 * @file big_int_test.cpp
 * @author Gabrielle Ching-Johnson
 * @brief This file runs unit tests for the big_int class
 * @version 0.1
 * @date Dec 7, 2021
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

/**
 * @brief Checks if each unit test passes or fails and writes output to log file
 * @param condition bool indication true if test passed and false if test failed
 * @param file file to write outcome of tests
 */
void check(bool condition, ofstream &file)
{
    if (condition)
    {
        file << " -> Passed\n";
        passed_counter++;
    }

    else
    {
        file << " -> Failed\n";
        failed_counter++;
    }
}

void number_tests_passed(ofstream &file)
{
    if (failed_counter == 0)
    {
        file << "All " << to_string(passed_counter) << " tests passed!!\n";
    }
    else
    {
        file << "Not all tests passed!\n"
             << "Number of tests passed: " << to_string(passed_counter) << "\n"
             << "Number of tests failed: " << to_string(failed_counter) << "\n";
    }
}

/**
 * @brief Prints desired string to a file
 * @param file The file to print line to
 */
void printline(string line, ofstream file)
{
    file << line;
}

/********************************************* Test Functions **********************************************/

/**
 * @brief Tests to check the default constructor and the copy constructor
 */
void check_other_constructors(ofstream &file)
{
    // Check the zero constructor
    file << "Default constructor check, big int set to zero ...";
    big_int zero_default;
    check(zero_default.at(0) == 0 && zero_default.coefficient_size() == 1, file);

    // Check copy constructor
}

/**
 * @brief Tests to check the constructor with an integer argument
 */
void check_integer_constructor(ofstream &file)
{
    // Check constructor with an integer argument
    file << "\nConstructor checks with Integer argument:\n";

    file << "Integer value less than 2^32 ...";
    big_int construct_int(4000);
    check(construct_int.at(0) == 4000 && construct_int.coefficient_size() == 1, file);

    file << "Integer value greater than 2^32 ...";
    big_int construct_int2(12345678910);
    check(construct_int2.at(0) == 3755744318 && construct_int2.at(1) == 2 && construct_int2.coefficient_size() == 2, file);

    file << "Negative integer value sign check ...";
    big_int neg_int(-256);
    check(neg_int.get_sign() == sign::NEGATIVE, file);

    file << "Negative integer value, check coefficients to be value as if integer was positive ...";
    check(neg_int.at(0) == 256 && neg_int.coefficient_size() == 1, file);
}

/**
 * @brief Tests to check the constructor with a string argument
 */
void check_string_constructor(ofstream &file)
{
    // Check constructor with base 10 string argument
    file << "\nBase 10 string argument constructor checks:\n";

    file << "Check for a positive value (without +) ...";
    big_int pos_string_int("256");
    check(pos_string_int.at(0) == 256 && pos_string_int.coefficient_size() == 1, file);

    file << "Check positive string with sign in front ...";
    check(pos_string_int.at(0) == 256 && pos_string_int.coefficient_size() == 1, file);

    file << "Check for negative value with - in front, that sign flag changes ...";
    big_int neg_string_int("-256");
    check(neg_string_int.get_sign() == sign::NEGATIVE, file);

    file << "Check negative string, that coefficients are values as if number was positive ...";
    check(neg_string_int.at(0) == 256 && neg_string_int.coefficient_size() == 1, file);

    file << "Check invalid input of the string argument ...\n"; /**TODO**/
}

/**
 * @brief Tests to the addition function of a big integer
 */
void check_addition(ofstream &file)
{
    big_int number1(45);
    big_int number2(-256);
    big_int sum = number1 + number2;
    check(sum.at(0) == 211 && sum.get_sign() == sign::NEGATIVE, file);
}

void check_subtraction_negation(ofstream &file)
{
    big_int positive(45);
    big_int positive2(256);
    big_int negative = -positive;
    file << "Checking the negation operator overload ...";
    check(negative.get_sign() == sign::NEGATIVE && negative.at(0) == 45, file);

    file << "Checking subtraction of 2 positive numbers with the 2nd being bigger ...";
    big_int sub = positive - positive2;
    check(sub.at(0) == 211 && sub.get_sign() == sign::NEGATIVE, file);
}

int main()
{
    /***************************************** Open Log File ********************************************/
    /* Referenced the following for filesystem class:
           https://en.cppreference.com/w/cpp/filesystem/path
           https://en.cppreference.com/w/cpp/filesystem/exists
    */

    // Check if log file name already exists, if so keep on appending a number to end of file name
    string log_file_name = "log_file_big_int_tests";
    string log_file_type = ".txt";
    string log_file = log_file_name + log_file_type;
    filesystem::path out_path(log_file);
    int file_num = 1;
    string original_log_file = log_file;
    while (filesystem::exists(out_path))
    {
        log_file = log_file_name + "_" + to_string(file_num) + log_file_type;
        out_path.replace_filename(log_file);
        file_num += 1;
    }

    // Open log file and check to ensure it opens
    ofstream log(log_file, ios::app);
    if (!log.is_open())
    {
        cout << "Error opening file " << log_file << "!\n";
        return -1;
    }

    // Inform file writing solution to
    if (original_log_file != log_file)
        cout << "Original Log File " << original_log_file << " already exists and thus writing test logs to file: " << log_file << "\n";
    else
        cout << "Writing test logs to " << log_file << "\n";

    /*********************************** Run tests and Write output to log files ****************************************/
    log << "***********Testing all constructors of the big_int class:***********\n";
    check_other_constructors(log);
    check_integer_constructor(log);
    check_string_constructor(log);
    number_tests_passed(log);

    log << "\n***********Testing printing to a string of the big_int class:***********\n";

    log << "\n***********Testing addition of the big_int class:***********\n";
    check_addition(log);

    log << "\n***********Testing subtraction and negation of the big_int class:***********\n";
    check_subtraction_negation(log);

    log << "\n***********Testing multiplication of the big_int class:***********\n";

    log << "\n***********Testing division of the big_int class:***********\n";

    if (failed_counter == 0)
    {
        log << "\nOverall " << to_string(passed_counter) << " tests passed!!\n";
    }
    else
    {
        log << "\nOverall not all tests passed!\n"
            << "Number of tests passed: " << to_string(passed_counter) << "\n"
            << "Number of tests failed: " << to_string(failed_counter) << "\n";
    }

    // Close log file and inform done writing
    log.close();
    cout << "Finished testing the big_int class, see the log file " << log_file << " for all the information.\n";
}