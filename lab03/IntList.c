 // IntList.c - Lists of integers
 // by Faiyam Islam 
 // Description: A C program which prints out a linked list in sorted order
 // (ascending order) given random values from the user. 

#include <assert.h>
#include <err.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sysexits.h>

#include "IntList.h"

// data structures representing IntList
struct IntListNode {
    int data;
    struct IntListNode *next;  // pointer to next node
};

struct IntListRep {
    int size;                  // number of elements in the list
    struct IntListNode *first; // pointer to node containing first value
    struct IntListNode *last;  // pointer to node containing last value
};

/**
 * Create a new, empty IntList.
 */
IntList IntListNew(void) {
    struct IntListRep *l = malloc(sizeof(*l));
    if (l == NULL) {
        err(EX_OSERR, "couldn't allocate IntList");
    }

    l->size = 0;
    l->first = NULL;
    l->last = NULL;
    return l;
}

/**
 * Release all resources associated with an IntList.
 */
void IntListFree(IntList l) {
    if (l == NULL) return;

    struct IntListNode *curr = l->first;
    while (curr != NULL) {
        struct IntListNode *temp = curr;
        curr = curr->next;
        free(temp);
    }
    free(l);
}

/**
 * Create an IntList by reading values from a file.
 * Assume that the file is open for reading.
 */
IntList IntListRead(FILE *in) {
    IntList l = IntListNew();

    int v;
    while (fscanf(in, "%d", &v) == 1) {
        IntListAppend(l, v);
    }
    return l;
}

/**
 * Display IntList as one integer per line on `stdout`.
 */
void IntListShow(IntList l) {
    IntListPrint(stdout, l);
}

/**
 * Create a new IntListNode with value v
 * (this function is local to this ADT)
 */
static struct IntListNode *newIntListNode(int v) {
    struct IntListNode *n = malloc(sizeof(*n));
    if (n == NULL) {
        err(EX_OSERR, "couldn't allocate IntList node");
    }
    n->data = v;
    n->next = NULL;
    return n;
}

/**
 * Append one integer to the end of an IntList.
 */
void IntListAppend(IntList l, int v) {
    assert(l != NULL);

    struct IntListNode *n = newIntListNode(v);
    if (l->first == NULL) {
        l->first = l->last = n;
    } else {
        l->last->next = n;
        l->last = n;
    }
    l->size++;
}

/**
 * Insert an integer into correct place in a sorted IntList.
 */
void IntListInsertInOrder(IntList l, int v) {
    
    // The assert command below confirms if the list is not empty. 
    assert(l != NULL);

    // Create a struct for the new node and allocate its memory from 
    // IntListNode. Create a second struct called curr which equals to 
    // the list l and points to the first node and create a struct for
    // previous node which equals to NULL.
    
    struct IntListNode *new_node = malloc(sizeof(struct IntListNode));
    struct IntListNode *curr_node = l->first; 
    struct IntListNode *prev_node = NULL;
    
    // The new node points to the data of the original list (IntListNode) 
    // which equals to v and the new node points to the next node equalling
    // to NULL.
    new_node->data = v;
    new_node->next = NULL;
    
    l->size++; // each element increases by one as we go through the list.

    if (l->first == NULL) {
        l->first = new_node;
        l->last = new_node;
    }   // We first check if the integer v is first element in the linked list.

    else {
        
        for (; curr_node != NULL && curr_node->data <= v;) {
            prev_node = curr_node; 
            curr_node = curr_node->next; 
        } // The new node is placed at a particular position and this for loop
        // allows that to occur.
        
        // These if statements will determine where v is inserted.
        if (prev_node == NULL) {
            new_node->next = l->first; 
            l->first = new_node; 
        } else {
            prev_node->next = new_node; 
            new_node->next = curr_node; 
        }
        
        // We have reached the end of the list and the
        // list points to the last node.
        if (curr_node == NULL) {
            l->last = new_node;
        }
    }
    
    return;
}

/**
 * Return the number of elements in an IntList.
 */
int IntListLength(IntList l) {
    assert(l != NULL);

    return l->size;
}

/**
 * Make a copy of an IntList.
 * New list should look identical to the original list.
 */
IntList IntListCopy(IntList l) {
    assert(l != NULL);

    IntList copy = IntListNew();
    for (struct IntListNode *curr = l->first;
            curr != NULL; curr = curr->next) {
        IntListAppend(copy, curr->data);
    }
    return copy;
}

/**
 * Make a sorted copy of an IntList.
 */
IntList IntListSortedCopy(IntList l) {
    // The assert command below confirms if the list is not empty. 
    assert(l != NULL);

    // Create struct called copy which equals to new linked list (function). 
    // Also create another structu for current node equal to linked list l
    // pointing to the first node (head). 
    struct IntListRep *copy = IntListNew();
    struct IntListNode *curr_node = l->first; 
    
    // While loop which allows the list to be copied and sorted with 
    // the current node pointing to the data of the linked list
    // assuming the condition that the current node is not equal, is true. 
    while (curr_node != NULL) {
        IntListInsertInOrder(copy, curr_node->data);
        curr_node = curr_node->next;    
    }

    return copy;
}

/**
 * Check whether an IntList is sorted in ascending order.
 * Returns `false` if list is not sorted, `true` if it is.
 */
bool IntListIsSorted(IntList l) {
    assert(l != NULL);

    // trivial cases, 0 or 1 items
    if (l->size < 2) {
        return true;
    }

    // scan list, looking for out-of-order pair
    for (struct IntListNode *curr = l->first;
            curr->next != NULL; curr = curr->next) {
        if (curr->next->data < curr->data) {
            return false;
        }
    }

    // nothing out-of-order, must be sorted
    return true;
}

/**
 * Check internal consistency of an IntList.
 */
bool IntListOK(IntList l) {
    assert(l != NULL);

    if (l->size == 0) {
        return (l->first == NULL && l->last == NULL);
    }

    // scan to (but not past) last node
    struct IntListNode *curr = l->first;
    int numNodes = 1; // at least one node
    while (curr->next != NULL) {
        numNodes++;
        curr = curr->next;
    }

    return (numNodes == l->size && curr == l->last);
}

/**
 * Display an IntList as one integer per line to a file.
 * Assume that the file is open for writing.
 */
void IntListPrint(FILE *out, IntList l) {
    assert(l != NULL);

    for (struct IntListNode *curr = l->first;
            curr != NULL; curr = curr->next) {
        fprintf(out, "%d\n", curr->data);
    }
}
