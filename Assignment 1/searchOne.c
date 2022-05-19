// COMP2521 Assignment 1
// Extra C file containing the functions: 
// total_files, TreeSearch, ListInsertion, tfidf_new.
// Faiyam Islam z5258151 (Date: 28/03/22)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Included headerfiles 
#include "invertedIndex.h"
#include "searchOne.h"

#define FILENAME_AND_WORDS 100

// Function which calculates total number of files in InvertedIndexNode. 
int total_files(InvertedIndexBST word) {
    FileList node = word->fileList;
    int num_of_files = 0; for (; node != 0; node = node->next) {
        num_of_files++;
    }
    return num_of_files;
}

// Function which discovers the node of the InvertedIndex tree and returns t.
// (from Week 3 lecture code on Trees & Binary Search Trees).
InvertedIndexBST TreeSearch(InvertedIndexBST t, char *word) {
    if (t == NULL) return NULL; // base case. 
    // Recursive cases: 
    // Determining in an alphabetised way of storing the words, using strcmp(). 
    else if (strcmp(word, t->word) > 0) return TreeSearch(t->right, word);
    else if (strcmp(word, t->word) < 0) return TreeSearch(t->left, word);
    else return t; 
}

// Function which inserts a node called new_tfidf into the list.
TfIdfList ListInsertion(TfIdfList list, char *file_name, double info_ret) {

    TfIdfList prev_node = NULL; // prev node of TfIdfList set to NULL.
    TfIdfList node = list; // node is set to the list of TfIdfList. 
	TfIdfList newList = tfidf_new(file_name, info_ret); // tfidf_new creates a new node containing a word. 

	if (list == NULL) {
		list = newList; return list; // return newList if it's NULL. 
	}

    
	for (; node != NULL; node = node->next) {
        if (node->tfIdfSum <= newList->tfIdfSum) { // Sum of newList must be either great or equal to sum of node. 
            break; 
		}
		prev_node = node;
	}

	if (node != NULL && newList->tfIdfSum == node->tfIdfSum) {
        // If the strings of the new node and the current node are less than 0, we 
        // need to use a temporary node to swap them. 
		if (strcmp(newList->filename, node->filename) < 0) {
			if (node == list) {
				TfIdfList temp_node = list;
				list = newList;
				list->next = temp_node;
			} else {
				newList->next = node;
				prev_node->next = newList;		
			}
		} else {
            if (node->next == NULL) {
                node->next = newList;
            } else if (node == list) {
                newList->next = list->next;
                list->next = newList; 
			} else {
				newList->next = node->next;
				node->next = newList;
			}
		}
		return list;
	}
    // Swapping nodes with same tfidf values
    // Source: https://tinyurl.com/6jyffdn2 
	if (node == list) {
		TfIdfList temp_node = list; 
		list = newList;
		list->next = temp_node;
	} else {
		newList->next = node;
		prev_node->next = newList;
	}
    return list;
} 

// Function which creates a new node which has a word. 
TfIdfList tfidf_new(char *file_name, double value) {
    // new_node from TfIdfList malloc'd.
    TfIdfList new_node = malloc(sizeof(*new_node));
    // The new node which points to the filename must have 
    // 100 times the memory of the file_name. 
    new_node->filename = malloc(FILENAME_AND_WORDS * sizeof(char));
        // First we check if the new node is NULL, if so 
        // print an error message. 
    if (new_node == NULL) {
        fprintf(stderr, "error: the new file is NULL\n"); exit(EXIT_FAILURE);
    }
    // The node pointing to the filename is copied to file_name. 
    // The node pointing to tfidfSum is assigned to tfidf value (double).
    strcpy(new_node->filename, file_name); new_node->tfIdfSum = value;
    // Then traverses until reaches NULL, returning the new node. 
    new_node->next = NULL; return new_node; 
}

