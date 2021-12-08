# Arbitrary Precision Integer Arithmetic - CSE 701 Project 3
**Gabrielle Ching-Johnson** \
December 07, 2021

## Background Information
In many computer programs the precision of an arithmetic operation is limited to the word size in which a computer has. In many modern day desktop computers a typical word size is 64 bits. Thus the largest number that can be represented in any program would be $2^{64} = 1.8446744*10^{19}$ if only looking at unsigned integers. However if one is using signed integers the largest range of values available is approximately $-9.2233720 *10^{18}$ to $9.2233720 *10^{18}$. There are also word sizes of 32 bits, 16 bits and 8 bits which have smaller ranges. However in many cases while doing an arithmetic operation, like addition and multiplication, the result could have a higher value that allowed by the computer. Thus the results in this case would not be accurate or correct.

Therefore this library was made in order to allow for any integer of any precision, while performing arithmetic operations.

## Library Basic Features

This library create a type called "big_int" and can model signed integers, thus both negative and positive integers can be portrayed. ***Input something about maximum size due to space on computer***

The operations that can occur on "big_int"s in the library are:
* addition
* subtraction
* multiplication
* division

## Constructors and Input

