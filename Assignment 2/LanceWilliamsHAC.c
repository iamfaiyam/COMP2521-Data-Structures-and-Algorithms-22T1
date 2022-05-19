// Lance-Williams Algorithm for Hierarchical Agglomerative Clustering
// COMP2521 Assignment 2
// Name: Faiyam Islam (z5258151)
// Date: 22/04/22

#include <assert.h>
#include <float.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"
#include "LanceWilliamsHAC.h"

#define INFINITY DBL_MAX

// Helper functions for LanceWilliamsHAC.c
double largerCluster(double alpha, double beta);
double smallerCluster(double clusterOne, double clusterTwo);
double calculateDistance(Graph g, Vertex v, Vertex w);
double calculateWeight(AdjList l, Vertex v);
void freeDistances(int num, double **edge);
Dendrogram dendrogramNode(int Node);

/**
 * Generates  a Dendrogram using the Lance-Williams algorithm (discussed
 * in the spec) for the given graph  g  and  the  specified  method  for
 * agglomerative  clustering. The method can be either SINGLE_LINKAGE or
 * COMPLETE_LINKAGE (you only need to implement these two methods).
 * 
 * The function returns a 'Dendrogram' structure.
 */
Dendrogram LanceWilliamsHAC(Graph g, int method) {
    // Similar to part 1 and 2, we initialise the number of vertices. 
    int numVertices = GraphNumVertices(g);
    
    // Initialises the distances of the 2D arrays. 
    double **distances = malloc(sizeof(*distances) * numVertices);
    // Allocates memory of the Dendrogram structure to newDendrogram. 
    Dendrogram *newDendrogram = malloc(sizeof(Dendrogram) * numVertices);

    for (int sourceVertex = 0; sourceVertex < numVertices; sourceVertex++) {
        distances[sourceVertex] = malloc(sizeof(*distances) * numVertices);
    }
    
    // Calculates the distances from the 2D arrays. 
    Vertex i = 0; 
    while (i < numVertices) {
        Vertex j = 0; 
        while (j < numVertices) {
            distances[i][j] = distances[j][i] = calculateDistance(g, i, j);
            j++;
        }
        i++;
    }

    int Dgram = 0; 
    while (Dgram < numVertices) {
        newDendrogram[Dgram] = dendrogramNode(Dgram);
        Dgram++;
    }
    int index = 0;
    
    // Need to update the dendrograms using the two methods mentioned in the spec. 
    int arraySize = numVertices;
    while (arraySize > 1) {

        // Initialises the single link. 
        double shortest = DBL_MAX;
        int placeA = -1;
        int placeB = -1;

        // Determines the shortest path between clusters. 
        for (int k = 0; k < numVertices; k++) {
            for (int m = k + 1; m < numVertices; m++) {
                if (distances[k][m] < shortest) {
                    shortest = distances[k][m];
                    placeA = k;
                    placeB = m;
                }
            }
        }
        
        // We will be use the general Lance-Williams formula for the two methods. 
        // Single linkage method which uses the maximum distance between each link. 
        if (method == 1) {
            for (int clusterIJ = 0; clusterIJ < numVertices; clusterIJ++) {
                for (int clusterK = 0; clusterK < clusterIJ; clusterK++) {
                    if (clusterIJ == placeA || clusterK == placeA) {
                        continue;
                    } else if (clusterK == placeB) {
                        distances[clusterIJ][clusterK] = distances[clusterK][clusterIJ] = 
                        smallerCluster(distances[clusterIJ][placeB], distances[clusterIJ][placeA]);
                    } else if (clusterIJ == placeB) {
                        distances[clusterIJ][clusterK] = distances[clusterK][clusterIJ] = 
                        smallerCluster(distances[placeB][clusterK], distances[placeA][clusterK]);
                    }
                }
            }  

        // Complete linkage method which uses the minimum distance between each link.           
        } else if (method == 2) {
            for (int clusterIJ = 0; clusterIJ < numVertices; clusterIJ++) {
                for (int clusterK = 0; clusterK < clusterIJ; clusterK++) {
                    if (clusterIJ == placeA || clusterK == placeA) {
                        continue;
                    } else if (clusterK == placeB) {
                        distances[clusterIJ][clusterK] = distances[clusterK][clusterIJ] = 
                        largerCluster(distances[clusterIJ][placeB], distances[clusterIJ][placeA]);
                    } else if (clusterIJ == placeB) {
                        distances[clusterIJ][clusterK] = distances[clusterK][clusterIJ] = 
                        largerCluster(distances[placeB][clusterK], distances[placeA][clusterK]);
                    }
                }
            }       
        }
        
        // Sets the distance array to infinity. 
        int i = 0; 
        while (i < numVertices) {
            int j = 0; 
            while (j < i) {
                if (i == placeA || j == placeA) {
                    distances[i][j] = distances[j][i] = DBL_MAX;
                }
                j++;
            }
            i++;
        }

        Dendrogram Dgram = newDendrogram[placeB];
        // All the vertices in the array are set to -1 first. 
        newDendrogram[placeB] = dendrogramNode(-1); 
        newDendrogram[placeB]->right = newDendrogram[placeA];
        newDendrogram[placeB]->left = Dgram;
        newDendrogram[placeA] = NULL;
        index = placeB;
        arraySize--;
    }
    Dendrogram dendrogram = newDendrogram[index];
    free(newDendrogram);
    freeDistances(numVertices, distances);
    return dendrogram;
}


/**
 * Frees all memory associated with the given Dendrogram structure.
 */
void freeDendrogram(Dendrogram d) { 
    if (d->left != NULL) {
        freeDendrogram(d->left);
    }
    if (d->right != NULL) {
        freeDendrogram(d->right);
    }
    free(d);
}

// Helper functions

// Function which compares two clusters and returns the larger one. 
double largerCluster(double clusterOne, double clusterTwo) {
    if (clusterOne == DBL_MAX) return clusterTwo; 
    if (clusterTwo == DBL_MAX) return clusterOne;
    if (clusterOne > clusterTwo) return clusterOne;
    if (clusterOne == DBL_MAX && clusterTwo == DBL_MAX) return DBL_MAX;
    return clusterTwo;
}

// Function which compares two clusters and returns the smaller one. 
double smallerCluster(double clusterOne, double clusterTwo) {
    if (clusterOne == DBL_MAX) return clusterTwo;
    if (clusterTwo == DBL_MAX) return clusterOne;
    if (clusterOne > clusterTwo) return clusterTwo;
    if (clusterOne == DBL_MAX && clusterTwo == DBL_MAX) return DBL_MAX;
    return clusterOne;
}

// Function which calculates the distance between two vertices. 
double calculateDistance(Graph g, Vertex v, Vertex w) {
    int adjVW = GraphIsAdjacent(g, v, w), adjWV = GraphIsAdjacent(g, w, v);
    double distanceVW = calculateWeight(GraphOutIncident(g, v), w);
    double distanceWV = calculateWeight(GraphInIncident(g, v), w);

    // If there are two links between v and w, we take the maximum of the two
    // weights and the distance between them is 1 / max(vw, wv). 
    if (adjVW && adjWV) {
        return 1 / largerCluster(distanceVW, distanceWV);
    // If there is one directed link between v and w with weight wt, the 
    // distance between them is 1 / wt. 
    } else {
        if (adjWV) {
            return 1 / distanceWV;
        } else if (adjVW) {
            return 1 / distanceVW;
        } else {
            return DBL_MAX;
        }
    }
}

// Function which calculates the weight of a vertex
// from the adjList. 
double calculateWeight(AdjList l, Vertex v) {
    AdjList vertex = l;
    while (vertex != NULL) {
        if (vertex->v == v) {
            return vertex->weight;
        }
        vertex = vertex->next; 
    }
    return DBL_MAX;
}

// Function which frees the calculated distances from each vertex. 
void freeDistances(int num, double **edge) {
    int e = 0; 
    while (e < num) {
        free(edge[e]);
        e++;
    }
    free(edge);
}

// Function which creates a new DNode to join the two clusters together. 
Dendrogram dendrogramNode(int Node) {
    Dendrogram newNode = malloc(sizeof(DNode));
    newNode->vertex = Node;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}