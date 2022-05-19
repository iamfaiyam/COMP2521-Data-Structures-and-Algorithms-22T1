// Centrality Measures API implementation
// COMP2521 Assignment 2
// Name: Faiyam Islam (z5258151)
// Date: 22/04/22

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "CentralityMeasures.h"
#include "Dijkstra.h"
#include "Graph.h"

// Helper functions for CentralityMeasures.c
static double countPaths(NodeData *array, int vN, int v0, int node, double *totalPaths);
static int recursiveDFS(Graph g, Vertex v, bool *vertex, int num);

double *closenessCentrality(Graph g) {
	int numVertices = GraphNumVertices(g);

	// Allocates the memory for the closeness centrality array. 
	double *dataCC = malloc(sizeof(double) * numVertices);

	// Allocates the memory for the results obtain from Dijkstra.
	NodeData **arrayCC = malloc(sizeof(struct NodeData) * numVertices);
	int v = 0; 
	while (v < numVertices) {
		arrayCC[v] = malloc(sizeof(struct NodeData) * numVertices);
		v++;
	}

	// Loops through the array for closeness centrality and stores the 
	// sum of all the shortest paths from each vertex. 
	int i = 0; 
	while (i < numVertices) {
		double sum = 0; 
		arrayCC[i] = dijkstra(g, i); // Must use Dijkstra for shortest paths. 
		int j = 0; 
		while (j < numVertices) {
			// If vertices reaches infinity then we add the sum of the 
			// shortest paths. 
			if (arrayCC[i][j].dist != INFINITY) {
				sum += arrayCC[i][j].dist;
			}
			j++;
		}
		dataCC[i] = sum; 
		i++;
	}

	
	bool *connectedNode = malloc(sizeof(*connectedNode) * numVertices);
	bool *node = connectedNode;
	int w = 0; 
	while (w < numVertices) {
		connectedNode[i] = 0; 
		w++;
	}

	int sourceVertex = 0; 
	while (sourceVertex < numVertices) {
		int destVertex = 0; 
		while (destVertex < numVertices) {
			connectedNode[destVertex] = 0;
			destVertex++;
		}
		int n = recursiveDFS(g, sourceVertex, connectedNode, 0);
		// The Wasserman and Faust formula to determine if there is more than one 
		// connected component to determine closeness centrality. 
		dataCC[sourceVertex] = ((n - 1) * (n - 1)) / ((numVertices - 1) * dataCC[sourceVertex]);
		sourceVertex++;
	}
	free(node);
	return dataCC;
}

double *betweennessCentrality(Graph g) {
	
	// Initialise similar to closenessCentrality as results are 
	// stored in the array which is then returned at the end. 
	int numVertices = GraphNumVertices(g);

	// Allocates the memory for the betweenness centrality array. 
	double *dataBC = malloc(sizeof(*dataBC) * numVertices);

	int i = 0; 
	while (i < numVertices) {
		NodeData *arrayBC = dijkstra(g, i); // Must use Dijkstra for shortest paths. 
		for (int j = 0; j < numVertices; j++) {
			if (arrayBC[j].dist == INFINITY) {
				continue;
			}
			// Allocates memory for the total number of shortest paths from 
			// node s to node t, given by σ_st.
			double *totalPaths = malloc(sizeof(double) * numVertices);

			// Allocates memory for the number of those paths that pass
			// through v, given by σ_st(v).
			double vPaths = countPaths(arrayBC, i, j, j, totalPaths);
			int path = 0; 
			while (path < numVertices) {
				dataBC[path] = totalPaths[path] / vPaths + dataBC[path];
				path++;
			}
		}
		i++;
	}
    return dataBC;
}

// Function which counts the number of cycles in a connected graph
// using a recursive DFS. 
// Reference: Lecture code under Week 7: Directed/Weighted Graphs. 
static int recursiveDFS(Graph g, Vertex v, bool *vertex, int num) {
	vertex[v] = 1;
	AdjList outgoing = GraphOutIncident(g, v);
	for (; outgoing != NULL; outgoing = outgoing->next) {
		if (vertex[outgoing->v] == 0) {
			num = recursiveDFS(g, outgoing->v, vertex, num);
		}
	}
	num++;
	return num; 
}

// Function which counts the total number of shortest paths from the initial vertex
// to the end vertex. This will help us calculate the number of those paths that pass 
// through v, indicated from the betweenness centrality formula. 
static double countPaths(NodeData *array, int vN, int v0, int node, double *totalPaths) {

	PredNode *currNode = array[node].pred;
	double directPath = 0;
	for (; currNode != NULL; currNode = currNode->next) {
		directPath = countPaths(array, vN, v0, currNode->v, totalPaths) + directPath;
	}

	// vN indicates end vertex, v0 is the starting point. 
	if (node == vN) {
		return 1;
	} else if (node != v0) {
		totalPaths[node] = directPath + totalPaths[node];
	}

	return directPath;
}

