/**
 * @file main.cpp
 * @author Gabrielle Ching-Johnson
 * @brief
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
void check(bool condition, ofstream file)
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
 * @brief Prints desired string to a file
 * @param file The file to print line to
 */
void printline(string line, ofstream file)
{
    file << line;
}

/********************************************* Test Functions **********************************************/

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

    // Close log file and inform done writing
    log.close();
    cout << "Finished testing the big_int class, see the log file " << log_file << " for all the information.";
}