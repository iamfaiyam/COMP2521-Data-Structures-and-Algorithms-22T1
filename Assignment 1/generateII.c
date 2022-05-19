// COMP2521 Assignment 1
// Extra C file containing the functions: 
// word_normalised, generateII, BSTreeInsert, generateII_two, 
// new_FL, freq_term, numwords_doc and match_list.
// Faiyam Islam z5258151 (Date: 28/03/22)

#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Included headerfiles 
#include "invertedIndex.h"
#include "generateII.h"

#define FILENAME_AND_WORDS 100

// Function which normalises words by converting all letters to 
// lowercase and removing the following punctuations marks from 
// the end of the words. In this function we consider 4 cases: 
// (1) Removing trailing whitespaces (2) Removing leading whitespaces. 
// (3) Removing punctuation marks at the end and (4) Converting characters to lower-case.
char *word_normalised(char *str) {
    
    int str_index, str_length = strlen(str), character = 0; 

    // Removal of trailing whitespaces. 
    for (str_index = -1; str[character] != '\0'; character++) {
        if (str[character] != '\n' && str[character] != '\t' && str[character] != ' ') {
            str_index = character; 
        }
    }
    
    str[character] = '\0';
    
    // Removal of leading whitespaces.
    for (str_index = 0; str[str_index] == '\n' || str[str_index] == '\t' ||
        str[str_index] == ' ';) {
    str_index++;
    }

    // Removal of punctuation at the end, including , . ? ; 
    // and null terminating value indicated with \0.
    if (str[str_length - 1] == ',' || str[str_length - 1] == '.' ||
        str[str_length - 1] == '?' || str[str_length - 1] == ';') {
        str[str_length - 1] = '\0';
    }
    
    // Converts characters to lower-case.
    for (character = 0; str[character] != '\0'; character++) {
        str[character] = tolower(str[character]); 
    }
    
    return str; 
}

// Generates inverted index (from Week 3 lecture code on Trees
// & Binary Search Trees).
InvertedIndexBST generateII() {
    return NULL;
}

// Function which inserts a new item into the InvertedIndex tree. 
// (from Week 3 lecture code on Trees & Binary Search Trees).
InvertedIndexBST BSTreeInsert(InvertedIndexBST t, char *word, 
char *collection_filename) {

    if (t == NULL) { // base case (empty item in tree)
        InvertedIndexBST t_node = generateII_two(word, collection_filename);
        t_node->fileList = new_FL(word, collection_filename);
        return t_node;
    // Inserts word into the right of the tree in InvertedIndex.
    } else if (strcmp(word, t->word) > 0) {
        t->right = BSTreeInsert(t->right, word, collection_filename);
    // Inserts word into the left of the tree in InvertedIndex.
    } else if (strcmp(word, t->word) < 0) {
        t->left = BSTreeInsert(t->left, word, collection_filename);
    // If the collection_filename does not exist in the tree, we 
    // have to add a new one. 
    } else {
        struct FileNode *file_node = new_FL(word, collection_filename);
        if (match_list(t->fileList, collection_filename) == false) {
            FileList new_file = t->fileList;
            // Loop the file pointing to next is not NULL, we assign it to file_node. 
            for (; new_file->next != NULL; new_file = new_file->next) {
                new_file->next = file_node;
            }
        } 
    }
    return t;
}

// Function which creates a new InvertedIndex if the tree is empty 
// from base case in BSTreeInsert function (from lab01 IntList.c).
InvertedIndexBST generateII_two(char *new_word, char *filename) {
    struct InvertedIndexNode *new_node = malloc(sizeof(struct InvertedIndexNode));
    assert(new_node != NULL);
    new_node->word = malloc(FILENAME_AND_WORDS);
    strcpy(new_node->word, new_word); 
    new_node->right = NULL; new_node->left = NULL; return new_node; 
}

// Creates new FileList called new_FL which contains new_word 
// (from lab01 IntList.c).
FileList new_FL(char *new_word, char *filename) {
    struct FileNode *new_node = malloc(sizeof(struct FileNode));
    assert(new_node != NULL); 
    new_node->filename = malloc(FILENAME_AND_WORDS); // new_node malloc'd to 100
    strcpy(new_node->filename, filename);

    // To simplify calculation of term frequency, we can create functions
    // which calculates the frequency of term t in d and number of words
    // in d (referenced from the assignment spec).
    new_node->tf = freq_term(new_word, filename) / numwords_doc(filename);
        new_node->next = NULL; return new_node;
}

// Function which calculates frequency of term t in d.
// Source: https://tinyurl.com/mrn7vcxr 
double freq_term(char *word, char *filename) {
    
    // Frequency of filenames and words which is 100 characters long. 
    char freq[FILENAME_AND_WORDS];
    double num = 0; 
    // File which opens and reads filename. 
    FILE *file = fopen(filename, "r");
    for (; fscanf(file, "%s", freq) != EOF; ) { // Scans filenames till EOF
        char *normalised_word = word_normalised(freq);
        // Comparing normalised words with the words in the file if 0,
        // then we increment the num.
        if (strcmp(normalised_word, word) == 0) {
            num++;
        }
    }
    fclose(file); return num;
}

// Function which determines the number of words in d.
double numwords_doc(char *filename) {
    
    // Same as in the function freq_term.
    char freq[FILENAME_AND_WORDS];
    double num = 0;
    
    // Opens file and reads it, then checks
    // frequency of the filename in the file list. 
    FILE *file = fopen(filename, "r");
    for (; fscanf(file, "%s", freq) == 1; num++);

    fclose(file); return num; 
}

// Function which determines if the name of the file is in 
// the FileList, need to use boolean in this case. 
// Source: https://tinyurl.com/2p969bwe 
bool match_list(struct FileNode *node, char *word) {
    for (; node != NULL; node = node->next) {
        // We check if the node in the filename, has the same
        // characters as the word in the file. 
        if (strcmp(node->filename, word) == 0) {
            return 0; // If there is a match.
        }
    }
    return 1; // If there is not a match. 
}
    
    
