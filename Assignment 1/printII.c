// COMP2521 Assignment 1 
// Extra C files containing the functions: 
// print_values, file_tf.
// Faiyam Islam z5258151 (Date: 28/03/22)

#include <stdio.h>
#include <stdlib.h>

// Included headerfiles 
#include "invertedIndex.h"
#include "printII.h"

// Function which goes through each node in the tree and prints tf values.
// Inspiration from Week 3 lecture code on Trees & Binary Search Trees.
void print_values(InvertedIndexBST t, FILE *file) {
    if (t != NULL) { // if file is empty
        print_values(t->left, file); // since a tree traverses starting left in infix order. 
        fprintf(file, "%s ", t->word);
        file_tf(t->fileList, file);
        print_values(t->right, file); // after all left nodes, tree then traverses right.
    }
}

// Function which prints the tf values based on every file. 
void file_tf(FileList fileList, FILE *tf_file) {
    for (FileList node = fileList; node != NULL; node = node->next) {
        // Ignore if a file has tf equal to 0 and print tfidf value 
        // to the file in 7 decimal places
        if (node->tf != 0) fprintf(tf_file, "%s (%.7f) ", node->filename, node->tf);
    }
    fprintf(tf_file, "\n"); // files are printed with a new line. 
}