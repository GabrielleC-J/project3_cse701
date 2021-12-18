/**
 * @file big_int_test.cpp
 * @author Gabrielle Ching-Johnson
 * @brief This file runs unit tests for the big_int class
 * @version 0.1
 * @date Dec 12, 2021
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
 */
void check_print(ofstream &file)
{
    string pos_int = "+12345678987654321";
    string neg_int = "-98765432123456789";
    big_int positive(pos_int);
    big_int negative(neg_int);

    file << "Check the printing of a positive big integer ...";
    check(pos_int == print_base10(positive), file);

    file << "Check the printing of a negative big integer ...";
    check(neg_int == print_base10(negative), file);
}

/**
 * @brief Tests the addition operator overload of a big integer
 */
void check_addition(ofstream &file)
{
    big_int small_neg(-45);
    big_int positive_1("38374635927640");
    big_int positive_2("63528394639737489393048304");
    big_int negative_1("-3638493734930473947493");

    file << "Check the addition of two positive numbers";
    big_int sum_pos = positive_1 + positive_2;
    check("+63528394639775864028975944" == print_base10(sum_pos), file);

    file << "Check the addition of two negative numbers";
    big_int sum_neg = negative_1 + small_neg;
    check("-3638493734930473947538" == print_base10(sum_neg), file);

    file << "Check the addition of a smaller negative value and bigger positive ...";
    big_int sum_pos_bigger = positive_2 + negative_1;
    check("+63524756146002558919100811" == print_base10(sum_pos_bigger), file);

    file << "Check the addition of a bigger negative value and smaller positive ...";
    big_int sum_neg_bigger = negative_1 + positive_1;
    check("-3638493696555838019853" == print_base10(sum_neg_bigger), file);
}

/**
 * @brief Tests for the subtraction and negation operator overloads for big integer
 */
void check_subtraction_negation(ofstream &file)
{
    big_int positive(45);
    big_int positive2(256);

    file << "Checking the negation operator overload ...";
    big_int negative = -positive;
    check(negative.get_sign() == sign::NEGATIVE && negative.at(0) == 45, file);

    file << "Checking subtraction of 2 positive numbers with the 2nd being bigger ...";
    big_int sub = positive - positive2;
    check("-211" == print_base10(sub), file);

    file << "Checking subtraction of 2 positive numbers where 2nd is smaller ...";
    big_int sub2 = positive2 - positive;
    check("+211" == print_base10(sub2), file);
}

/**
 * @brief Tests the multiplication operator overload for big integer
 */
void check_multiplication(ofstream &file)
{
    big_int small_1(98);
    big_int small_2(4);
    string small_sol = "+392";
    big_int pos_1("7657346809103847");
    big_int pos_2("675335738394");
    string two_pos_solution = "+5171279961465086275171001718"; // pos_1*pos_2
    big_int neg_1(-50);
    big_int neg_2("-394785643744");
    string two_neg_sol = "+19739282187200";               // neg_1*neg_2
    string neg_pos_sol = "-3023010589403126517641883168"; // pos_1 * neg_2

    file << "Checking multiplication of basic integers ...";
    check(small_sol == print_base10(small_1 * small_2), file);

    file << "checking the multiplication of two positive big numbers ...";
    check(two_pos_solution == print_base10(pos_1 * pos_2), file);

    file << "Checking the multiplication of two negative big integers ...";
    check(two_neg_sol == print_base10(neg_1 * neg_2), file);

    file << "Checking the multiplication of a negative and positive big integer ...";
    check(neg_pos_sol == print_base10(pos_1 * neg_2), file);
}

/**
 * @brief Test the division operator overload for big integers
 */
void check_division(ofstream &file)
{
    big_int small_1(95);
    big_int small_2(6);
    big_int pos_1("56789738193238029839");
    big_int pos_2("893293838329865");
    big_int pos_3("339475993");
    big_int neg_1("-464836");
    big_int neg_2("-63573848");

    file << "Checking division for basic integers ...";
    check("+15" == print_base10(small_1 / small_2), file);

    file << "Checking division for dividend < divisor ...";
    check("+0" == print_base10(neg_1 / pos_2), file);

    file << "Checking division by zero exception thrown ...";
    try
    {
        big_int zero;
        pos_1 / zero;
        // Failed to throw exception
        check(false, file);
    }
    catch (const invalid_argument &e)
    {
        // exception thrown
        check(true, file);
    }

    file << "Checking division for negative dividend and positive divisor integers ...";
    check("-4893" == print_base10(neg_1 / small_1), file);

    file << "Checking division for positive dividend and negative divisor integers ...";
    check("-5" == print_base10(pos_3 / neg_2), file);

    file << "Checking division of two positive numbers ...";
    check("+63573" == print_base10(pos_1 / pos_2), file);
}

/**
 * @brief Test the < and > operator overload for big integers
 */
void check_less_and_greater_than(ofstream &file)
{
    big_int pos_1("235632");
    big_int pos_2("46383");
    big_int neg_1("-3654836");
    big_int neg_2("-76327");
    big_int more_digits("3764839473947839");

    file << "Checking less than for two positive numbers ...";
    check(pos_2 < pos_1, file);

    file << "Checking greater than for exact same numbers as two positive less than ...";
    check(pos_1 > pos_2, file);

    file << "Checking less than for two negative numbers ...";
    check(neg_1 < neg_2, file);

    file << "Checking less than for negative and positive numbers ...";
    check(neg_1 < pos_1, file);

    file << "Checking less than for integers with different number of digits in base 2^32 ...";
    check(pos_1 < more_digits, file);
}

/**
 * @brief Test != and == operator overload for big integers
 */
void check_not_equals_and_equals(ofstream &file)
{
    big_int pos_1("3764839473947839");
    big_int equal_pos_1("+3764839473947839");
    big_int neg_1("-3764839473947839");
    big_int pos_2("46383");

    file << "Checking equals for the same value ...";
    check(pos_1 == equal_pos_1, file);

    file << "Checking not equals for the same value but different sign ...";
    check(pos_1 != neg_1, file);

    file << "Checking not equals for two different values ...";
    check(pos_2 != pos_1, file);
}

void read_values(vector<string> &value_vec, const string &line, const uint8_t &line_num)
{
    string value;
    istringstream line_stream(line);
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

    // Check all values are numbers in the vector or boolean
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
            if ((integers[0] == '+' || integers[0] == '-') && integers.size() != 1)
            {
                start++;
            }
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
    vector<string> numbers;
    vector<string> sums;
    vector<string> diffs;
    vector<string> products;
    vector<string> quotients;
    vector<string> greater_less;
    try
    {
        uint8_t line_counter = 0;
        for (uint8_t i = 1; getline(operations_input, input_line) || i < 7; i++)
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
    check_print(log);
    number_tests_passed(log);
    update_counters();

    log << "\n***********Testing addition of the big_int class:***********\n";
    cout << "Testing Addition\n";
    check_addition(log);
    number_tests_passed(log);
    update_counters();

    log << "\n***********Testing subtraction and negation of the big_int class:***********\n";
    cout << "Testing Subtraction and Negation\n";
    check_subtraction_negation(log);
    number_tests_passed(log);
    update_counters();

    log << "\n***********Testing multiplication of the big_int class:***********\n";
    cout << "Testing Multiplication\n";
    check_multiplication(log);
    number_tests_passed(log);
    update_counters();

    log << "\n***********Testing division of the big_int class:***********\n";
    cout << "Testing Division\n";
    check_division(log);
    number_tests_passed(log);
    update_counters();

    log << "\n************Testing less than and greater than operator of big_int class:************\n";
    cout << "Testing Less than and Greater than Operator\n";
    check_less_and_greater_than(log);
    number_tests_passed(log);
    update_counters();

    log << "\n************Testing equals and not equals operator of big_int class:************\n";
    cout << "Testing Equals and Not Equals Operator\n";
    check_not_equals_and_equals(log);
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