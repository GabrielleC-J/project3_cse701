/**
 * @file big_int_test.cpp
 * @author Gabrielle Ching-Johnson
 * @brief This file runs unit tests for the big_int class
 * @version 0.2
 * @date Dec 18, 2021
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
uint8_t total_passed = 0;
uint8_t total_failed = 0;

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

/**
 * @brief Writes total number of tests passed in a section and total failed
 * @param file file to write outcome of tests
 */
void number_tests_passed(ofstream &file)
{
    if (failed_counter == 0)
    {
        file << "\nAll " << to_string(passed_counter) << " tests in the section passed!!\n";
    }
    else
    {
        file << "\nNot all tests passed!\n"
             << "Number of tests passed: " << to_string(passed_counter) << "\n"
             << "Number of tests failed: " << to_string(failed_counter) << "\n";
    }
}

/**
 * @brief Updates total counters
 */
void update_counters()
{
    total_passed += passed_counter;
    total_failed += failed_counter;
    passed_counter = 0;
    failed_counter = 0;
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
    file << "Copy Constructor check ...";
    big_int to_copy(12345678910);
    check(to_copy.at(0) == 3755744318 && to_copy.at(1) == 2 && to_copy.coefficient_size() == 2, file);
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

    file << "Check invalid input of the string argument ...";
    try
    {
        big_int failed_int("-123457j234");
        // Failed to throw exception
        check(false, file);
    }
    catch (const invalid_argument &e)
    {
        // exception thrown
        check(true, file);
    }
}

/**
 * @brief Tests for the print function of a big integer
 * @param numbers A vector of 4 big integers, the 1st two are positive and the 2nd two are negative
 * @param file The log file to print results to
 */
void check_print(const vector<string> &numbers, ofstream &file)
{
    // Add the + sign if solution does not contain it
    string exact_pos;
    if (numbers[0].at(0) != '+')
    {
        exact_pos = "+" + numbers[0];
    }
    else
    {
        exact_pos = numbers[0];
    }
    string exact_neg = numbers[2];
    big_int positive(exact_pos);
    big_int negative(exact_neg);

    file << "Check the printing of a positive big integer ...";
    check(exact_pos == print_base10(positive), file);

    file << "Check the printing of a negative big integer ...";
    check(exact_neg == print_base10(negative), file);
}

/**
 * @brief Tests the addition operator overload of a big integer
 * @param numbers A vector of 4 big integers, the 1st two are positive and the 2nd two are negative
 * @param sum_vals A vector of strings containing the solutions to the summation of big integers
 * @param file The log file to print results to
 */
void check_addition(const vector<big_int> &numbers, const vector<string> &sum_vals, ofstream &file)
{
    file << "Random Big Integers Tests:\n";
    file << "Check the addition of two positive numbers";
    big_int sum_pos = numbers[0] + numbers[1];
    check(sum_vals[0] == print_base10(sum_pos), file);

    file << "Check the addition of two negative numbers";
    big_int sum_neg = numbers[2] + numbers[3];
    check(sum_vals[1] == print_base10(sum_neg), file);

    file << "Check the addition of the 1st random positive and negative value ...";
    big_int sum_pos_one = numbers[0] + numbers[2];
    check(sum_vals[2] == print_base10(sum_pos_one), file);

    file << "Check the addition of the 2nd random negative and positive value ...";
    big_int sum_neg_two = numbers[1] + numbers[3];
    check(sum_vals[3] == print_base10(sum_neg_two), file);
}

/**
 * @brief Tests for the subtraction and negation operator overloads for big integer
 * @param numbers A vector of 4 big integers, the 1st two are positive and the 2nd two are negative
 * @param differences A vector of strings containing the solutions to the difference of big integers
 * @param file The log file to print results to
 */
void check_subtraction_negation(const vector<big_int> &numbers, const vector<string> &differences, ofstream &file)
{
    file << "Random Big Integers Tests:\n";
    file << "Checking the negation operator overload for positive value ...";
    big_int negative = -numbers[0];
    check(negative.get_sign() == sign::NEGATIVE, file);

    file << "Checking subtraction of 2 positive numbers ...";
    big_int sub = numbers[0] - numbers[1];
    check(differences[0] == print_base10(sub), file);

    file << "Checking subtraction of 1st negative and positive random integers ...";
    big_int sub2 = numbers[2] - numbers[0];
    check(differences[1] == print_base10(sub2), file);

    file << "Checking subtraction of 2 negative numbers ...";
    big_int sub3 = numbers[2] - numbers[3];
    check(differences[2] == print_base10(sub3), file);
}

/**
 * @brief Tests the multiplication operator overload for big integer
 * @param numbers A vector of 4 big integers, the 1st two are positive and the 2nd two are negative
 * @param products A vector of strings containing the solutions to the multiplication of big integers
 * @param file The log file to print results to
 */
void check_multiplication(const vector<big_int> &numbers, const vector<string> &products, ofstream &file)
{
    file << "Random Big Integers Tests:\n";
    file << "checking the multiplication of two positive big numbers ...";
    check(products[0] == print_base10(numbers[0] * numbers[1]), file);

    file << "Checking the multiplication of two negative big integers ...";
    check(products[1] == print_base10(numbers[2] * numbers[3]), file);

    file << "Checking the multiplication of the 1st random negative and positive big integer ...";
    check(products[2] == print_base10(numbers[0] * numbers[2]), file);
}

/**
 * @brief Test the division operator overload for big integers
 * @param numbers A vector of 4 big integers, the 1st two are positive and the 2nd two are negative
 * @param quotients A vector of strings containing the solutions to the division of big integers
 * @param file The log file to print results to
 */
void check_division(const vector<big_int> &numbers, const vector<string> &quotients, ofstream &file)
{
    big_int pos_2("893293838329");
    big_int neg_1("-464836");

    file << "Hard Coded tests:\n";
    file << "Checking division for dividend value < divisor value ...";
    check("+0" == print_base10(neg_1 / pos_2), file);

    file << "Checking division for dividend > divisor value ...";
    check("-1921739" == print_base10(pos_2 / neg_1), file);

    file << "\nRandom Big Integers Tests:\n";

    file << "Checking division by zero exception thrown ...";
    try
    {
        big_int zero;
        numbers[0] / zero;
        // Failed to throw exception
        check(false, file);
    }
    catch (const invalid_argument &e)
    {
        // exception thrown
        check(true, file);
    }

    file << "Checking division for two positive integers ...";
    check(quotients[0] == print_base10(numbers[0] / numbers[1]), file);

    file << "Checking division for  two negative numbers ...";
    check(quotients[1] == print_base10(numbers[2] / numbers[3]), file);

    file << "Checking division of the 1st negative and positive random integer ...";
    check(quotients[2] == print_base10(numbers[2] / numbers[0]), file);
}

/**
 * @brief Test the < and > operator overload for big integers
 * @param numbers A vector of 4 big integers, the 1st two are positive and the 2nd two are negative
 * @param bool_sols A vector of strings containing the solutions to the comparison of big integers
 * @param file The log file to print results to
 */
void check_less_and_greater_than(const vector<big_int> &numbers, const vector<string> &bool_sols, ofstream &file)
{
    big_int pos_1("235632");
    big_int more_digits("3764839473947839");
    bool solution;

    file << "Hard Coded tests:\n";

    file << "Checking less than for integers with different number of digits in base 2^32 ...";
    check(pos_1 < more_digits, file);

    file << "\nRandom Big Integers Tests:\n";
    file << "Checking less than for two positive numbers ...";
    istringstream(bool_sols[0]) >> boolalpha >> solution; // convert string solution to boolean
    check(((numbers[0] < numbers[1]) == solution), file);

    file << "Checking greater than for exact same numbers as two positive less than ...";
    istringstream(bool_sols[1]) >> boolalpha >> solution; // convert string solution to boolean
    check(((numbers[1] > numbers[0]) == solution), file);

    file << "Checking less than for two negative numbers ...";
    istringstream(bool_sols[2]) >> boolalpha >> solution; // convert string solution to boolean
    check(((numbers[2] < numbers[3]) == solution), file);

    file << "Checking less than for negative and positive numbers ...";
    istringstream(bool_sols[3]) >> boolalpha >> solution; // convert string solution to boolean
    check(((numbers[2] < numbers[0]) == solution), file);
}

/**
 * @brief Test != and == operator overload for big integers
 * @param numbers A vector of 4 big integers, the 1st two are positive and the 2nd two are negative
 * @param file The log file to print results to
 */
void check_not_equals_and_equals(const vector<big_int> &numbers, ofstream &file)
{

    file << "Checking equals for the same value ...";
    check(numbers[0] == numbers[0], file);

    file << "Checking not equals for the same value but different sign ...";
    check(numbers[0] != numbers[2], file);

    file << "Checking not equals for two different values ...";
    check(numbers[0] != numbers[1], file);
}

/**
 * @brief Read input values into desired vector and check for invalid input
 * @param value_vec
 */
void read_values(vector<string> &value_vec, const string &line, const uint8_t &line_num)
{
    string value;
    istringstream line_stream(line);
    // input each value in line to vector
    while (getline(line_stream, value, ','))
    {
        value_vec.push_back(value);
    }

    // Check number of input values per line
    uint64_t size = value_vec.size();
    if (line_num == 1 || line_num == 2 || line_num == 6)
    {
        if (size != 4)
        {
            throw invalid_argument("Invalid number of integers, expected 4 but got " + to_string(size) + " in line " + to_string(line_num) + "\n");
        }
    }
    else
    {
        if (size != 3)
        {
            throw invalid_argument("Invalid number of integers, expected 3 but got " + to_string(size) + " in line " + to_string(line_num) + "\n");
        }
    }

    // Check all values are numbers in the vector or boolean for 6th line
    for (const string &integers : value_vec)
    {
        if (line_num == 6)
        {
            if (integers != "true" && integers != "false")
            {
                throw invalid_argument("Invalid bool provided in line 6 expected either true or false but got " + integers + "\n");
            }
        }
        else
        {
            string::const_iterator start = integers.begin();
            // Check 1st character if + or - sign and move iterator start to next char if sign is there and string is not a single char
            if ((integers[0] == '+' || integers[0] == '-') && integers.size() != 1)
            {
                start++;
            }
            // Check rest of the values that they are digits
            if (!all_of(start, integers.end(), ::isdigit))
            {
                throw invalid_argument("Invalid integer provided in line " + to_string(line_num) + "\n");
            }
        }
    }
}

int main()
{
    /***************************************** Read Input File ******************************************/
    string input_file = "../test_input.txt";
    ifstream operations_input(input_file);
    if (!operations_input.is_open())
    {
        cout << "Error opening input file"
             << "../ test_input.txt ";
        return -1;
    }

    string input_line;
    vector<string> numbers;      // 4 big_int strings
    vector<big_int> big_numbers; // the 4 big integers as type big_int
    vector<string> sums;         // sums of the integers
    vector<string> diffs;        // differences of the integers
    vector<string> products;     // products of the integers
    vector<string> quotients;    // quotients of the integers
    vector<string> greater_less; // boolean for < and >

    // Read 6 lines from input
    try
    {
        uint8_t line_counter = 0;
        for (uint8_t i = 1; getline(operations_input, input_line); i++)
        {
            switch (i)
            {
            case 1:
                read_values(numbers, input_line, i);
                break;
            case 2:
                read_values(sums, input_line, i);
                break;
            case 3:
                read_values(diffs, input_line, i);
                break;
            case 4:
                read_values(products, input_line, i);
                break;
            case 5:
                read_values(quotients, input_line, i);
                break;
            case 6:
                read_values(greater_less, input_line, i);
                break;
            default:
                break;
            }
            line_counter++;
        }

        if (line_counter != 6)
        {
            throw invalid_argument("Invalid number of lines read, expected 6 lines but only read " + to_string(line_counter) + "\n");
        }
    }
    catch (invalid_argument &exception)
    {
        cout << exception.what();
        return -1;
    }

    // update the big_int vector with values
    for (const string &integers : numbers)
    {
        big_numbers.push_back(big_int(integers));
    }

    /***************************************** Open Log File ********************************************/
    /* Referenced the following for filesystem class:
           https://en.cppreference.com/w/cpp/filesystem/path
           https://en.cppreference.com/w/cpp/filesystem/exists
    */

    // Check if log file name already exists, if so keep on appending a number to end of file name
    string log_file_name = "log_file_big_int_tests";
    string log_file_type = ".txt";
    string log_file = log_file_name + log_file_type;
    string file_path = log_file;
    filesystem::path out_path(file_path);
    int file_num = 1;
    string original_log_file = log_file;
    while (filesystem::exists(out_path))
    {
        log_file = log_file_name + "_" + to_string(file_num) + log_file_type;
        file_path = log_file;
        out_path.replace_filename(file_path);
        file_num += 1;
    }

    // Open log file and check to ensure it opens
    ofstream log(file_path, ios::app);
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
    cout << "Testing Constructors\n";
    check_other_constructors(log);
    check_integer_constructor(log);
    check_string_constructor(log);
    number_tests_passed(log);
    update_counters();

    log << "\n***********Testing printing to a string of the big_int class:***********\n";
    cout << "Testing printing to base 10\n";
    check_print(numbers, log);
    number_tests_passed(log);
    update_counters();

    log << "\n***********Testing addition of the big_int class:***********\n";
    cout << "Testing Addition\n";
    check_addition(big_numbers, sums, log);
    number_tests_passed(log);
    update_counters();

    log << "\n***********Testing subtraction and negation of the big_int class:***********\n";
    cout << "Testing Subtraction and Negation\n";
    check_subtraction_negation(big_numbers, diffs, log);
    number_tests_passed(log);
    update_counters();

    log << "\n***********Testing multiplication of the big_int class:***********\n";
    cout << "Testing Multiplication\n";
    check_multiplication(big_numbers, products, log);
    number_tests_passed(log);
    update_counters();

    log << "\n***********Testing division of the big_int class:***********\n";
    cout << "Testing Division\n";
    check_division(big_numbers, quotients, log);
    number_tests_passed(log);
    update_counters();

    log << "\n************Testing less than and greater than operator of big_int class:************\n";
    cout << "Testing Less than and Greater than Operator\n";
    check_less_and_greater_than(big_numbers, greater_less, log);
    number_tests_passed(log);
    update_counters();

    log << "\n************Testing equals and not equals operator of big_int class:************\n";
    cout << "Testing Equals and Not Equals Operator\n";
    check_not_equals_and_equals(big_numbers, log);
    number_tests_passed(log);
    update_counters();

    // Write overall total passed and failed
    log << "\n*********************************************\n";
    if (total_failed == 0)
    {
        log << "Overall " << to_string(total_passed) << " tests passed!!\n";
    }
    else
    {
        log << "\nOverall not all tests passed!\n"
            << "Number of tests passed: " << to_string(total_passed) << "\n"
            << "Number of tests failed: " << to_string(total_failed) << "\n";
    }

    // Close log file and inform done writing
    log.close();
    cout << "Finished testing the big_int class.\n"
         << to_string(total_passed) << " tests passed and " << to_string(total_failed) << " tests failed.\nSee the log file " << log_file << " for all the information.\n";
}