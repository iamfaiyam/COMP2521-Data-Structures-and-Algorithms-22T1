// COMP2521 Assignment 1
// A C program used for information retrieval from various files. 
// Faiyam Islam z5258151 (Date: 28/03/22)

#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Included headerfiles 
// PART 1 
#include "invertedIndex.h"
#include "generateII.h"
#include "printII.h"
// PART 2 
#include "searchOne.h"
#include "searchMany.h"

#define FILENAME_AND_WORDS 100 

// Part 1 - Inverted Index
// Description:
// This function creates an inverted index from a collection of files and 
// compute the term frequency (tf) for each word. 
InvertedIndexBST generateInvertedIndex(char *collectionFilename) {
   
    // Filenames and words will be at most 100 characters long. 
    char scanned_word[FILENAME_AND_WORDS], file_name[FILENAME_AND_WORDS];
    // All scanned words in the InvertedIndexBST must be normalised. 
    char *normalisation = word_normalised(scanned_word);

    // Generates inverted index (from Week 3 lecture code on Trees
    // & Binary Search Trees).
    InvertedIndexBST node = generateII(); 
    // Opening collectionFilename assigned to new_file.
    FILE *new_file = fopen(collectionFilename, "r");
    // Checking if new_file is NULL.
    if (new_file == NULL) {
        fprintf(stderr, "error: the file that was opened is NULL\n");
        exit(1);
    }
    // Scans if the new file is in file_name.
    for (; fscanf(new_file, "%s", file_name) == 1; ) {
        // Opens and reads the the file. 
        FILE *another_file = fopen(file_name, "r");
        if (new_file == NULL) {
            fprintf(stderr, "error: the file that was opened is NULL\n");
            exit(1);
        }   
        // If new file is in file_name, we check if the word is in 
        // the file of the inverted index.
        for (; fscanf(another_file, "%s", scanned_word) == 1; ) {
            node = BSTreeInsert(node, normalisation, file_name);
        }
    }
    return node;
}        
 
// Function which prints inverted index to to a file given filename. 
void printInvertedIndex(InvertedIndexBST t, char *filename) {
    // Open filename which is an empty file for writing. 
    FILE *file = fopen(filename, "w");
    // Goes through tree and prints tf value. 
    print_values(t, file);
    fclose(file); return;
}

// Function which frees all memory associated with 
// the given inverted index. (from Week 3 lecture code on 
// Trees & Binary Search Trees).
void freeInvertedIndex(InvertedIndexBST t) {
    if (t != NULL) { // inverted index tree cannot be NULL. 
        freeInvertedIndex(t->right); freeInvertedIndex(t->left); free(t);
    } 
}

// Part 2 - Information Retrieval
// Description: 
// Returns TfIdf value for each given searchWord, where the list is descending order
// and if there are multiple files with same value, it's ascending. 
TfIdfList searchOne(InvertedIndexBST tree, char *searchWord, int D) {
    InvertedIndexBST word = TreeSearch(tree, searchWord); 
    // Initialising key variables for tfidf calculation. 
    // value is the tfidf value. 
    // total_docs is total documents in a file.
    // sum_files is the number of files.
    // idf is the idf value which requires log10 from math.h.
    double value = 0, total_docs = D, sum_files = total_files(word), idf = log10(total_docs/sum_files);

    if (tree == NULL) return NULL; // If the file is NULL. 
    // The node in FileList equals to the word which traverses across the tree till reaching NULL. 
    FileList node = word->fileList;
    TfIdfList list = NULL;

    for (; node != NULL; node = node->next) { 
        value = node->tf; 
        value = value * idf; // value is set to the idf value calculated above.
        list = ListInsertion(list, node->filename, value); // after idf calculation, insert into the list. 
    }      
    return list;
}

// Function which takes an inverted index, one or more search words, and the total 
// number of documents in the collection, and returns a list of all the files that
// contains at least one of the given words. 
TfIdfList searchMany(InvertedIndexBST tree, char *searchWords[], int D) {
    // searchManyList is a list of all the files containing words which 
    // we equate to searchOne function but with an array of searchWords. 
    TfIdfList searchManyList = searchOne(tree, searchWords[0], D);
    // Initialise an insert variable and make sure the first index of the 
    // search words is not NULL (or empty). 
    int insert = 0;
    if (searchWords[0] == NULL) return NULL;

    // Starting from the first node in the file, assuming not NULL.
    for (int word = 1; searchWords[word] != NULL; word++) {
        // Now we assign listOne which is a file to searchOne containing array of searchWords.
        TfIdfList listOne = searchOne(tree, searchWords[word], D);
        for (TfIdfList curr_node = listOne; curr_node != NULL; curr_node = curr_node->next) {
            // Node is assigned to searchManyList containing array of searchWords. 
            for (TfIdfList node = searchManyList; node != NULL; node = node->next) {
                if (strcmp(node->filename, curr_node->filename) == 0) { // Use strcmp to compare words in files. 
                    // Sum of tfidf in the files are added together. 
                    node->tfIdfSum = curr_node->tfIdfSum + node->tfIdfSum; insert = 1; 
                }
            }
            // If insert is still 0, we insert a new node pointing to the filename and tfidf sum. 
            if (insert == 0) {
                searchManyList = ListInsertion(searchManyList, curr_node->filename, curr_node->tfIdfSum);
            }
            insert = 0;
        }
    }
    // searchManyList is then sorted in descending or ascending order, depending on the tfidf value. 
    searchManyList = sortList(searchManyList); return searchManyList; 
}

// Function which frees all memory associated with the given list. 
// (Implemented from lab01 IntList.c)
void freeTfIdfList(TfIdfList list) {
    if (list == NULL) return;
    
    TfIdfList node = list;
    for (; node != NULL; node = node->next) {
        TfIdfList temp_node = node;
        free(temp_node);
    }
}
