// COMP2521 Assignment 1 
// Extra header files containing the functions: 
// word_normalised, generateII, BSTreeInsert, generateII_two, 
// new_FL, freq_term, numwords_doc and match_list.
// Faiyam Islam z5258151 (Date: 28/03/22)

#ifndef GENERATEII_H
#define GENERATEII_H

#include <stdio.h>
#include <stdlib.h>

#include "invertedIndex.h"

// Prototype function 1: 
// Function which normalises words by converting all letters to 
// lowercase and removing the following punctuations marks from 
// the end of the words. 
char *word_normalised(char *str); 

// Prototype function 2: 
// Generates inverted index (from Week 3 lecture code on Trees
// & Binary Search Trees).
InvertedIndexBST generateII();

// Prototype function 3:
// Function which inserts a new item into the InvertedIndex tree. 
// (from Week 3 lecture code on Trees & Binary Search Trees).
InvertedIndexBST BSTreeInsert(InvertedIndexBST tree, char *word, 
char *filename);

// Prototype function 4: 
// Function which creates a new InvertedIndex if the tree is empty 
// from base case in BSTreeInsert function (from lab01 IntList.c)
InvertedIndexBST generateII_two(char *new_word, char *filename);

// Prototype function 5: 
// Function which creates a new FileList called file_node
// (from lab01 IntList.c)
FileList new_FL(char *new_word, char *filename);

// Prototype function 6:
// Function which calculates frequency of term t in d.
double freq_term(char *word, char *filename);

// Prototype function 7: 
// Function which determines the number of words in d.
double numwords_doc(char *filename);

// Prototype function 8: 
// Function which determines if the name of the file is in 
// the FileList, need to use boolean in this case. 
bool match_list(struct FileNode *head, char *str);

#endif
