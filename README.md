# Arbitrary Precision Integer Arithmetic - CSE 701 Project 3
**Gabrielle Ching-Johnson** \
December 12, 2021

## Background Information
In many computer programs the precision of an arithmetic operation is limited to the word size in which a computer has. In many modern day desktop computers a typical word size is 64 bits. Thus the largest number that can be represented in any program would be $2^{64} = 1.8446744*10^{19}$ if only looking at unsigned integers. However if one is using signed integers the largest range of values available is approximately $-9.2233720 *10^{18}$ to $9.2233720 *10^{18}$. There are also word sizes of 32 bits, 16 bits and 8 bits which have smaller ranges. However in many cases while doing an arithmetic operation, like addition and multiplication, the result could have a higher value than allowed by the computer. Thus the results in this case would not be accurate or correct.

Every number can be presented in the following manner:

$x = x_{n-1}b^{n-1} + ... + x_{2}b^{2} + x_{1}b^{1} + x_{0}b^{0}$

where b represents the base the number is in and the x's coefficients are the digits in the specified base.

This library will take advantage of the form above to save memory space by utilizing a different base, instead of base 10, to store the digits of an integer.

An example in base 10:  <p align="center">$12345 = 1 * 10^4 + 2 * 10^3 + 3 * 10^2 + 4 * 10 + 5$</p>


Therefore this library was made in order to allow for any integer of any precision, while performing arithmetic operations.

## Library Basic Features

This library creates a type called `big_int` and can model signed integers, thus both negative and positive integers can be portrayed. 

The operations that can occur on "big_int"s in the library are:
* addition
* subtraction
* negation
* multiplication
* division
* less than
* greater than
* not equal to
* equal to

## Data Structure and Base
### Base
Utilizing the form specified in the background information for integers, instead of storing integers in base 10, this library will store the integers in base $2^{32}$ to save memory space.

### Data Structure for Digits storage
The data structure used to store the digits is a vector of unsigned 32 bit integers called coefficients: `vector<uint32_t> coefficients`. To account for overflow when doing arithmetic operations on the digits, these operations will use 64 bit intermediate values since each coefficient/digit can only have a value less than the base.

The storage of the digits will be in little endian form, thus the least significant digit (LSD) will be at index 0 in the vector and the most significant digit (MSD) will be at index (vector_size - 1). This storage method makes it easier for the addition, subtraction and multiplication operations.

For example in base 10 for integer 12345: the LSD: 5 and MSD: 1 and the vector (in base 10) would be {5, 4, 3, 2, 1}

## Sign of the Integer
The library contains an enum class called `sign` with two values `POSITIVE` AND `NEGATIVE`.

Each `big_int` object contains a private variable called `integer_sign` of type `sign` that will indicate whether the integer is positive or negative. This variable will be updated upon construction and when operations occur. For example for a negative integer the variable will be:
```cpp
sign integer_sign = sign::NEGATIVE
```
*Note: That for negative values, the digits are stored as if they are positive, but the sign will indicate that the value is actually negative*

## Constructors and Input
1. Default constructor : This will create a big integer that has the value of zero.
   ```cpp
    // Construct a big int with value zero
    big_int zero;
    ```
2. Integer constructor: This constructor will take an input of a signed 64 bit integer as the argument and create a `big_int` with the specified value.
    ```cpp
    // Construct a big int with value -65
    big_int integer(-65);
    ```
3. String constructor: This constructor will take a string argument as the input and create a `big_int` with the value specified by the string in base $2^{32}$. 
    * The very first character in the string can be a `-` indicating a negative number, a `+` indicating a positive number or nothing indicating a positive number.
    * All other characters in the string must be digits from 0 to 9.
    * This will throw an exception called `invalid_string_integer` if an invalid character (that is not a digit) is provided
    ```cpp
    // Construct a big int with value +12345678956
    big_int positive("+12345678956");

    // Construct a big int with value -12345678956
    big_int negative("-12345678956");

    // Construct a big int with value 12345678956, which will be the same as the positive big_int created above
    big_int positive("12345678956");

    // Exception will be thrown for the following attempt
    big_int except("3273947j6568");
    ```
4. Copy Constructor: This constructor takes a `big_int` object as an argument and will copy it to create a new `big_int` with the same values and sign.
    ```cpp
    // Copy the following big_int
    big_int to_copy(45);
    // Create a copy:
    big_int copy(to_copy);
    ```

## Printing to a string in base 10

The function `print_base10()` is a friend function of the `big_int` class. The method takes a `big_int` as the input argument and will print the value of the big integer, that is stored in base 2^32^, as a base 10 integer.

Example:
```cpp
big_int integer("2345566")
cout << print_base10(integer) << "\n

big_int integer2("+56385769384754834")
cout << print_base10(integer2) << "\n

big_int integer3("-465788474")
cout << print_base10(integer3) << "\n
```
The output:
```none
+2345566
+56385769384754834
-465788474
```

### The Method
In order to get the base 10 digits, first divide the big_int by 10 and the remainder of this division will be the least significant digit of the integer in base 10. Then continuously divide by 10 and get the remaining digits using the remainder until the division by ten results in 0.

The helper private functions of the class `big_int` that are used by this function are `divide32() and remainder32()`. The `divide32()` function takes an unsigned 32 bit integer argument and divides the big integer by this value and thus will be used when dividing by 10. While the `remainder32()` takes an unsigned 32 bit integer argument and outputs the remainder of the big integer divided by this value and thus will be used when finding the remainder by dividing by 10.[^1]

## Member Functions

### Get Sign

The `get_sign()` member function gets the sign of the current `big_int`.

Example:
```cpp
big_int pos(5); // pos.integer_sign is sign::POSITIVE

pos.get_sign() == sign::POSITIVE //evaluates to true
```

### Negate Member Function

The negate member function will negate the current `big_int`.

For example:
```cpp
big_int pos(5); // pos.integer_sign is sign::POSITIVE

pos.negate()

pos.get_sign() == sign::NEGATIVE //equates to true
```

### At Member Function

The `.at(index)` takes a 64 bit unsigned integer as the argument and return the digit in the coefficient vector at the index supplied.

### Coefficient Size Member Function

The `.coefficient_size()` member function retrieves the number of digits of the big integer that is in base 32.

### The = operator overload

This overloads the = operator to assign one big int to another. It takes a `big_int` as input and returns a reference to a new big_int that is a copy of the input argument (ie a deep copy).

## Operations on Big_ints

### Addition Operator

The addition operator will take two `big_int` as arguments, adds them and return a new big_int whose value is their sum.

As in adding base 10 integers, this function will go digit by digit, starting from the LSD at index 0 of the coefficient vector, and add the digits. It will also account for the carry produced by the addition and add that to the next digit addition.

For example:
```cpp
big_int positive_1("38374635927640");
// Note the vector of digits: positive_1.coefficient = {3398105176, 8934} 
big_int positive_2("63528394639737489393048304");
// Note: positive_2.coefficient = {3584940784, 747951344, 3443881}
```
Notice that when you add the digits at index 0 you will get an overflow:
```none
3398105176 + 3584940784 = 6983045960
thus the sum value at this index would be: 2688078664 (which is 6983045960 - base)
while the carry value is 1 which would be added to the next addition for index 1 
(Note that a carry of 1 represents 2^32 in base 10)
```

The sum would be:
```cpp
big_int sum = positive_1 + positive_2;
// Where sum.coefficient = {2688078664, 747960279, 3443881}
```

**Addition including negative numbers**

In order to add negative numbers the radix complement is used. In this case, the radix complement is the 2^32's complement.[^2]

When a negative number is being added in the addition, the big_int is converted to its radix complement and then added.

* If two negative values are added then they are both converted to their radix complement and then added.
* If the result of an addition is in fact negative, then the result will be in radix complement form and must be converted (ie get the radix complement of the radix complement).
* When adding a negative and positive number where the positive is greater than the negative, then by using the radix complement an extra carry must be deleted at the last index.[^2]

Example:

```cpp
big_int int1(45);  // coefficient vector {45}
big_int int2(-6);  //coefficient vector {6}  -> radix complement vector {4292967290}
```
If you sum the radix complement of -6 and 45 you will get 4294967335 > 2^32.
Thus there would be a carry of 1 (base 2^32)

Therefore the coefficient vector of the sum would be {39, 1}, which is why we need to erase the 1

The output should be:
```cpp
big_int sum = int1 + int_2  // with coefficient vector {39}
print_base10(sum) = "+39"
```

### Negation Operator

The negation operator takes in a `big_int` as the only argument and produces a new `big_int` object that is the negation of the input.

Example:
```cpp
big_int positive(65);
big_int negative = -positive;
```
Thus if you print both big integers in base 10 the strings you would get are:

```cpp
print_base10(positive) -> // "+65"
print_base10(negative) -> // "-65"
```

### The Subtraction Operator

The Subtraction operator takes two `big_int`s as arguments and produces a `big_int` object that's value is the difference of the two.

This operator just employs the addition and negation operator.

Example:
```cpp
big_int int_a(65);
big_int int_b(4);

big_int diff = int_a - int_b
// diff.coefficient == {61}
```

### Multiplication Operator

The Multiplication operator takes two `big_int`s as inputs and outputs a `big_int` object whose value is the product of the two.

The operator employs basic integer multiplication by going digit by digit of one integer and multiplying it by all digits of the other integers and summing up the outputs.

Example:

```cpp
big_int int_a(55);
big_int int_b(5);

big_int product = int_a * int_b;

print_base10(product) == "+275"; // true
```

### Division Operator

The division operator takes two `big_int`s as inputs and outputs a `big_int` object whose value is the quotient of the two. The first `big_int` input argument is the dividend and the 2nd is the divisor. Thus it would be dividend/divisor. The quotient would be an integer without the remainder.

The operator employs basic long division.

Example:
```cpp
big_int int_a(59);
big_int int_b(5);

big_int quotient = int_a / int_b; //59/5 = 11.8
print_base10(quotient) == "+11";
```

The division operator does employ an invalid argument exception, `division_by_zero` if an attempt to divide by a `big_int` that is the value 0.

### "<" Operator

The less than operator takes in two `big_int` arguments and returns a boolean stating if the 1st argument is less than the second.

Example

```cpp
big_int int_a(45);
big_int int_b(-4);

if (int_b < int_a)
    cout << "b is smaller"
```

Expected Output:
```none
b is smaller
``` 

### ">" Operator

The greater than operator takes in two `big_int` arguments and returns a boolean stating true if the 1st argument is larger than the second. 

This operator just employs the "<" operator by stating if the 2nd argument is less than the 1st.

Example

```cpp
big_int int_a(45);
big_int int_b(-4);

if (int_a > int_b)
    cout << "a is bigger"
```

Expected Output:
```none
a is bigger
```

### "!=" Operator

This operator takes in tow `big_int` arguments and returns a boolean stating true if the arguments do not have the same value.

Example

```cpp
big_int int_a(45);
big_int int_b(-4);

if (int_a != int_b)
    cout << "a is not equal to b"
```

Expected Output:
```none
a is not equal to b
```

## Testing of Library
The file ***big_int_test.cpp*** contains all the unit tests for the `big_int` class.

There are a total of 39 tests which tests all the arithmetic operations, the constructors and the print_base10 function. All the tests on the constructors and a couple of other tests are hard coded to explore certain boundaries of the operations.

The rest of the tests are based off of 4 random big integer numbers that will be read from an input file called ***test_input.txt***. This file was created using a python script called ***create_random_nums.py***. The following tests will utilize the 4 integers and their solutions need to be read from the input:

1. sum of two numbers:
   *  2 positive numbers
   *  2 negative numbers
   *  a negative and positive number
   *  a different positive and negative number.
2. difference of two numbers:
   * 2 positive numbers
   * a negative and positive number
   * 2 negative numbers
3. product of two numbers:
   * 2 positive numbers
   * 2 negative numbers
   * a positive and negative number
4. quotient of two numbers:
   * 2 positive numbers
   * 2 negative numbers
   * a negative nad positive number
5. comparison of 2 numbers (greater than and less than)
   * 2 positive numbers (using less than)
   * 2 positive numbers (using greater than and the same numbers as above)
   * 2 negative numbers
   * a negative and positive number

The tests for the printing, != operator and == operator utilize the 4 random numbers however do not need their solutions to be calculated by the python script.

### Python Script - create_random_nums.py

This python script is used to choose 4 random big numbers: 2 positive and 2 negative integers. Currently the script picks two positive numbers in the range [0, $2^{1024}$] and two negative numbers in the range[$-2^{1024}$, 0).  The four integers will be used to calculate the results for the tests (stated above) for addition, subtraction, multiplication, division and the comparison operators.

The order and structure of the numbers and results can be found under the input file section below.

### Input file - test_input.txt
The input file test_input.txt will be read by the c++ test file to read the 4 random integers and the results of the operations found using the python script. All the results will contain either a + or a - sign as they will be compared with the printing output of the big integer which contains the sign. Note that the 2 positive integers in the 1st line do not need the sign, but can have it. The file will contain 6 lines:

1. The 4 random integers (pos_1, pos_2, neg_1, neg_2)
2. Sum solutions
3. Difference solutions
4. Product solutions
5. Quotient solutions
6. Greater and less than test solutions

The structure of the file is as follows:

* pos_1, pos_2, neg_1, neg_2
* pos_1 + pos_2, neg_1 + neg_2, pos_1 + neg_1, neg_2 + pos_2
* pos_1 - pos_2, neg_1 - pos_1, neg_1 - neg_2
* pos_1 * pos_2, neg_1 * neg_2, pos_1 * neg_1
* pos_1 / pos_2, neg_1 / neg_2, neg_1 / pos_1
* pos_1 < pos_2, pos_2 > pos_1, neg_1 < neg_2, neg_1 < pos_1

An example input file with simple numbers would be:

```none
+45,+2,-13,-5
+47,-18,+32,-3
+43,-58,-18
+90,+65,-585
+22,+2,+0
false,false,true,true
```

### Log file

A log of all the tests will be written to a file named ***log_file_big_int_tests.txt***. If this file already exists a number will be appended to the end of the name until the file does not exist, thus not deleting any existing log files.

## References 
[^1]: Hansen, P. (1994). Multiple-length division revisited: A tour of the minefield. Software: Practice And Experience, 24(6), 579-601. doi: 10.1002/spe.4380240605

[^2]: Weik M.H. (2000) radix-minus-one complement. In: Computer Science and Communications Dictionary. Springer, Boston, MA. https://doi.org/10.1007/1-4020-0613-6_15429