// Dijkstra API implementation
// COMP2521 Assignment 2
// Name: Faiyam Islam (z5258151)
// Date: 22/04/22

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "Dijkstra.h"
#include "Graph.h"
#include "PQ.h"

// Helper functions for Dijkstra.c 
static PredNode *insertPredNode(PredNode *predHead, PredNode *addNode);
static void freePredNode(PredNode *pred);

NodeData *dijkstra(Graph g, Vertex src) {

	// Initialise the shortest path. 
	NodeData *shortestPath = malloc(sizeof(NodeData) * GraphNumVertices(g));

	// Set all indexes to infinity since they have not been reached yet, and 
	// these distances will be updated throughout the code. 
	Vertex index = 0; 
	while (index < GraphNumVertices(g)) {
		shortestPath[index].dist = INFINITY;
		shortestPath[index].pred = NULL;
		index++;
	}
	shortestPath[src].dist = 0; 
	
	// Construct a vSet that contains vertices of the graph. 
	PQ vSet = PQNew();
	// The predecessors are set to 0 initially. 
	PQInsert(vSet, src, 0);

	while (!PQIsEmpty(vSet)) {
		// Dequeue item with smallest value in priority queue. 
		Vertex vertex = PQDequeue(vSet);

		// Investigate the item's neighbours. 
		AdjList currList = GraphOutIncident(g, vertex);

		while (currList != NULL) {

			if (shortestPath[currList->v].dist > shortestPath[vertex].dist + currList->weight) {
				shortestPath[currList->v].dist = shortestPath[vertex].dist + currList->weight; 
				PredNode *predNode = malloc(sizeof(NodeData));
				predNode->v = vertex;
				predNode->next = NULL;
				shortestPath[currList->v].pred = predNode;
				PQInsert(vSet, currList->v, shortestPath[vertex].dist + currList->weight); 

			} else if (shortestPath[currList->v].dist == shortestPath[vertex].dist + currList->weight) {
				PredNode *pred = malloc(sizeof(NodeData));
				pred->v = vertex;
				pred->next = NULL;
				shortestPath[currList->v].pred = insertPredNode(shortestPath[currList->v].pred, pred); 
			}

			currList = currList->next;
		}
	}
	PQFree(vSet);
	return shortestPath;
}

// Function which frees all memory associated with NodeData. 
void freeNodeData(NodeData *data, int numVertices) {
	// Frees vertices in the predecessor list.
	int vertices; 
	while (vertices < numVertices) {
		freePredNode(data[vertices].pred);
		vertices++;
	}
	free(data);
}

// Helper functions

// Function which adds PredNode to the predecessor list. 
static PredNode *insertPredNode(PredNode *predHead, PredNode *addNode) {
	// If list is empty, then insert node. 
	if (predHead == NULL) {
		return addNode;
	} else if (addNode->v < predHead->v) {
		addNode->next = predHead;
		return addNode;
	}

	PredNode *currNode = predHead;
	while (currNode->next != NULL && currNode->next->v < addNode->v) {
		currNode = currNode->next;
	}
	addNode->next = currNode->next;
	currNode->next = addNode;
	return predHead;
}

// Function which frees the PredNode associated with the graph. 
static void freePredNode(PredNode *pred) {
	while (pred != NULL) {
		PredNode *tempNode = pred;
			pred = pred->next;
		free(tempNode);
	}
}
