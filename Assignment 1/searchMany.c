// COMP2521 Assignment 1
// Extra C file containing the function: 
// sortList.
// Faiyam Islam z5258151 (Date: 28/03/22)

#include <stdio.h>
#include <stdlib.h>

// Included headerfiles 
#include "invertedIndex.h"
#include "searchOne.h"
#include "searchMany.h"

// Function which will sort the list in descending order of summation of tf-idf
// values and if they are the same then it's ordered alphabetically 
// in filename in ascending order. 
TfIdfList sortList(TfIdfList tflist) {
    
    TfIdfList node = tflist, sortedList = NULL;
    for (; node != NULL; node = node->next) {
        // List is sorted in ascending order through ListInsertion function. 
        sortedList = ListInsertion(sortedList, node->filename, node->tfIdfSum);
    }
    if (tflist == NULL) return NULL; // if the list is empty. 
    return sortedList; 
}