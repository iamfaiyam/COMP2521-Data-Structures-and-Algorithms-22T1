// COMP2521 Assignment 1 
// Extra header files containing the functions: 
// print_values, file_tf.
// Faiyam Islam z5258151 (Date: 28/03/22)

#ifndef PRINTII_H
#define PRINTII_H

#include <stdio.h>
#include <stdlib.h>

// Prototype function 1: 
// Function which goes through each node in the tree and prints tf values.
void print_values(InvertedIndexBST t, FILE *file);

// Prototype function 2: 
// Function which prints the tf values based on every file. 
void file_tf(FileList fileList, FILE *tf_file);

#endif

