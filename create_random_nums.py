# @Author: Gabrielle Ching-Johnson
# @date 17 Dec 2021
# @brief This is a script that chooses 4 random big integers and finds solutions to arithmetic operations

import random

file_name = "test_input.txt"

# chose four random numbers 
max = 2**1024 + 1
min = -2**1024
pos1 = random.randint(0,max + 1)
pos2 = random.randint(0,max + 1)
neg1 = random.randint(min, 0)
neg2 = random.randint(min, 0)

numbers = [pos1, pos2, neg1, neg2]

# operations on numbers
pos_sum = pos1 + pos2
neg_sum = neg1 + neg2
pos_neg_sum = pos1 + neg1
neg_pos_sum = neg2 + pos2

sums = [pos_sum, neg_sum, pos_neg_sum, neg_pos_sum]

sub_pos = pos1 - pos2
sub_neg_pos = neg1 - pos1
sub_neg_neg = neg1 - neg2

sub = [sub_pos, sub_neg_pos, sub_neg_neg]

mult_pos = pos1 * pos2
mult_neg = neg1 * neg2
mult_pos_neg = pos1 * neg1

multiply = [mult_pos, mult_neg, mult_pos_neg]

div_pos = int(pos1 / pos2)
div_neg = int(neg1 / neg2)
div_pos_neg = int(neg1 / pos1)

div = [div_pos, div_neg, div_pos_neg]

great_less = [pos1 < pos2, pos2 > pos1, neg1 < neg2, neg1 < pos1]

# print values to file 

input_file = open(file_name, "w")

# print 2 positive then 2 negative

input_file.write(",".join("+" + str(i) if i >= 0 else str(i) for i in numbers) + "\n")

# print addition solutions

input_file.write(",".join("+" + str(i) if i >= 0 else str(i) for i in sums) + "\n")

# print negation and subtraction solutions

input_file.write(",".join("+" + str(i) if i >= 0 else str(i) for i in sub) + "\n")

# print multiplication solutions

input_file.write(",".join("+" + str(i) if i >= 0 else str(i) for i in multiply) + "\n")

# print division solutions

input_file.write(",".join("+" + str(i) if i >= 0 else str(i) for i in div) + "\n")

# print < and > solutions

input_file.write(",".join("true" if i else "false" for i in great_less))

input_file.close()