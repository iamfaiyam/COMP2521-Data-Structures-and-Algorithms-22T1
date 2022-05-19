// COMP2521 Assignment 1 
// Extra header file containing the function:
// sortList
// Faiyam Islam z5258151 (Date: 28/03/22) 

#ifndef SEARCHMANY_H
#define SEARCHMANY_H

#include "invertedIndex.h"

// Prototype function 1: 
// Function which will sort the list in descending order of summation of tf-idf
// values and if they are the same then it's ordered alphabetically (using strcmp)
// in filename in ascending order.
TfIdfList sortList(TfIdfList tflist);

#endif