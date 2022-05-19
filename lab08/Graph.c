// Implementation of the Undirected Weighted Graph ADT
// Uses an adjacency matrix
// Faiyam Islam (z5258151)

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"
#include "PQ.h"

struct graph {
    int nV;         // #vertices
    int nE;         // #edges
    double **edges; // adjacency matrix storing positive weights
                    // 0 if nodes not adjacent
};

static bool doHasCycle(Graph g, Vertex v, Vertex prev, bool *visited);
static int  validVertex(Graph g, Vertex v);

////////////////////////////////////////////////////////////////////////

Graph GraphNew(int nV) {
    assert(nV > 0);

    Graph g = malloc(sizeof(*g));
    if (g == NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
    }

    g->nV = nV;
    g->nE = 0;

    g->edges = malloc(nV * sizeof(double *));
    if (g->edges == NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < nV; i++) {
        g->edges[i] = calloc(nV, sizeof(double));
        if (g->edges[i] == NULL) {
            fprintf(stderr, "error: out of memory\n");
            exit(EXIT_FAILURE);
        }
    }

    return g;
}

void GraphFree(Graph g) {
    for (int i = 0; i < g->nV; i++) {
        free(g->edges[i]);
    }
    free(g->edges);
    free(g);
}

////////////////////////////////////////////////////////////////////////

int GraphNumVertices(Graph g) {
    return g->nV;
}

bool GraphInsertEdge(Graph g, Edge e) {
    assert(validVertex(g, e.v));
    assert(validVertex(g, e.w));
    assert(e.v != e.w);
    assert(e.weight > 0.0);

    if (g->edges[e.v][e.w] == 0.0) {
        g->edges[e.v][e.w] = e.weight;
        g->edges[e.w][e.v] = e.weight;
        g->nE++;
        return true;
    } else {
        return false;
    }
}

bool GraphRemoveEdge(Graph g, Vertex v, Vertex w) {
    assert(validVertex(g, v));
    assert(validVertex(g, w));

    if (g->edges[v][w] != 0.0) {   // edge e in graph
        g->edges[v][w] = 0.0;
        g->edges[w][v] = 0.0;
        g->nE--;
        return true;
    } else {
        return false;
    }
}

double GraphIsAdjacent(Graph g, Vertex v, Vertex w) {
    assert(validVertex(g, v));
    assert(validVertex(g, w));
    
    return g->edges[v][w];
}

void GraphShow(Graph g) {
    printf("Number of vertices: %d\n", g->nV);
    printf("Number of edges: %d\n", g->nE);
    for (int v = 0; v < g->nV; v++) {
        for (int w = v + 1; w < g->nV; w++) {
            if (g->edges[v][w] != 0.0) {
                printf("Edge %d - %d: %lf\n", v, w, g->edges[v][w]);
            }
        }
    }
}

bool GraphHasCycle(Graph g) {
    bool *visited = calloc(g->nV, sizeof(bool));
    assert(visited != NULL); // lazy error checking
    
    for (int v = 0; v < g->nV; v++) {
        if (!visited[v] && doHasCycle(g, v, v, visited)) {
            free(visited);
            return true;
        }
    }

    free(visited);
    return false;
}

static bool doHasCycle(Graph g, Vertex v, Vertex prev, bool *visited) {
    visited[v] = true;
    for (int w = 0; w < g->nV; w++) {
        if (g->edges[v][w] != 0.0) {
            if (!visited[w]) {
                if (doHasCycle(g, w, v, visited)) {
                    return true;
                }
            } else if (w != prev) {
                return true;
            }
        }
    }
    return false;
}

////////////////////////////////////////////////////////////////////////
// Your task

Edge ReturnEdge(Vertex v, Vertex w, double weight) {
    Edge edge_return = {.v = v,.w = w,.weight = weight};
    return edge_return;
}

Edge FindEdge(Graph MST, PQ pq, double *vert) {
    // We navigate PQ here and check if the vertices are used in 
    // the varible vert. 
    // If the vertices are used then the value equal to 1.
    // If the vertices are unused then the value equal to 0.

    PQ temp = PQNew();
    Edge e; 

    bool edge_found = false; 
    while (edge_found == false) {
        if (PQIsEmpty(pq) == true) {
            e = ReturnEdge(-1, -1, -1);
            break;
        }
        e = PQExtract(pq);

        if(e.weight > 0 && e.v != e.w && ((vert[e.v] == 0) != (vert[e.w] == 0))) {
            vert[e.w] = 1; 
            vert[e.v] = 1;
            edge_found = true; 
        } else { 
            PQInsert(temp, e);
        }
    }
    // Insert unused edges back into temporary varible. 
    Edge temp_edge;
    while (!PQIsEmpty(temp)) {
        temp_edge = PQExtract(temp);
        PQInsert(pq, temp_edge);
    }
    PQFree(temp);
    return e;
}

void use_edge(PQ pq, Graph g) {
    Edge temp; 
    for (int i = 0; i < g->nV; i++) {
        for (int j = 0; j < g->nV; j++) {
            if (i > j && i != j && g->edges[i][j] > 0) {
                temp = ReturnEdge(i, j, g->edges[i][j]);
                PQInsert(pq, temp);
            }
        }
    }
}

Graph GraphMST(Graph g) {
    
    // Minimum Spanning Tree using Prim's algorithm.
    // Pseudocode taken from the lecture slides: 
    /*
    PrimMST(G):
    |  Input  graph G with n nodes
    |  Output a minimum spanning tree of G
    |
    |  MST=empty graph
    |  usedV={0}
    |  unusedE=edges(g)
    |  while |usedV| < n do
    |  |  find e=(s,t,w) in unusedE such that {
    |  |     s in usedV and t not in usedV 
    |  |       and w is min weight of all such edges
    |  |  }
    |  |  MST = MST U {e}
    |  |  usedV = usedV U {t}
    |  |  unusedE = unusedE \ {e}
    |  end while
    |  return MST
    */

    // Will be using a priority queue. 
    int nV = g->nV;
    Graph MST = GraphNew(nV);
    double *vert = calloc(nV, sizeof(double));
    // If used vertices are NULL then we print an error. 
    if (vert == NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(1);
    }

    PQ pq = PQNew(); // Unused edge E. 
    use_edge(pq, g); 
    Edge E; 
    int size_usedV = 0;

    vert[0] = 1; 
    while (size_usedV < nV) {
        E = FindEdge(g, pq, vert);
        
        if (E.weight != -1) {
            GraphInsertEdge(MST, E); 
            size_usedV++;
        } else { 
            break;
        }
    }

    // Freeing the used vertices and pq.
    free(vert);
    PQFree(pq);

    if (MST-> nV - 1 != MST->nE) {
        return NULL;
    }
    return MST;
}

////////////////////////////////////////////////////////////////////////

static int validVertex(Graph g, Vertex v) {
    return v >= 0 && v < g->nV;
}

