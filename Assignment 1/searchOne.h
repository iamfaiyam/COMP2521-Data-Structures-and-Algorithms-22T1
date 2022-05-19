// COMP2521 Assignment 1 
// Extra header files containing the functions: 
// total_files, TreeSearch, ListInsertion, tfidf_new.
// Faiyam Islam z5258151 (Date: 28/03/22)

#ifndef SEARCHONE_H
#define SEARCHONE_H

#include <stdio.h> 
#include <stdlib.h> 

#include "invertedIndex.h"

// Prototype function 1: 
// Function which calculates total number of files in InvertedIndexNode. 
int total_files(InvertedIndexBST word);

// Prototype function 2: 
// Function which discovers the node of the InvertedIndex tree and returns t.
// (from Week 3 lecture code on Trees & Binary Search Trees).
InvertedIndexBST TreeSearch(InvertedIndexBST t, char *word);

// Prototype function 3: 
// Function which inserts a node called new_tfidf into the list.
TfIdfList ListInsertion(TfIdfList L, char *filenName, double TfIdf);

// Prototype function 4: 
// Function which creates a new node with has a word. 
TfIdfList tfidf_new(char *file_name, double TfIdf);

#endif