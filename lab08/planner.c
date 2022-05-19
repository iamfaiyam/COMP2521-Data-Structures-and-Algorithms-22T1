// Algorithms to design electrical grids
// Faiyam Islam (z5258151)

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"
#include "Place.h"
#include "PQ.h"

typedef struct graph *Grid; 
typedef Graph Grid; 
struct graph { 
    int nV;     // number of vertices
    int nE;     // number of edges
    double **edges;     // adjacency matrix storing positive weights
                        // 0 if nodes are not adjacent
};

////////////////////////////////////////////////////////////////////////
// Your task

/**
 * Designs  a minimal cost electrical grid that will deliver electricity
 * from a power plant to all the given cities. Power lines must be built
 * between cities or between a city and a power plant.  Cost is directly
 * proportional to the total length of power lines used.
 * Assumes  that  numCities  is at least 1 (numCities is the size of the
 * cities array).
 * Stores the power lines that need to be built in the given  powerLines
 * array, and returns the number of power lines stored. Assumes that the
 * powerLines array is large enough to store all required power lines.
 */

Edge ReturnEdgePlanner(Vertex v, Vertex w, double weight) {
    Edge ret = {.v = v,.w = w,.weight = weight};
    return ret;
}

Place ReturnPlace(int j, int nV, Place cities[], Place powerPlant) {
    if (j == nV - 1) {
        return powerPlant; 
    }
    return cities[j];
}

PowerLine ReturnPowerLine(Place p1, Place p2) {
    PowerLine power_return = {.p1=p1 ,.p2 = p2};
    return power_return;
}

// We calculate the weight between powerlines using the distance formula.
int CalculateWeight(int i, int j, Place cities[], Place powerPlant, int 
numCities) {
    int x1, x2, y1, y2;

    if (i < numCities && j == numCities) {
        x1 = cities[i].x; 
        y1 = cities[i].y;
        x2 = powerPlant.x;
        y2 = powerPlant.y;
    } else if (i < numCities && j < numCities) {
        x1 = cities[i].x;
        y1 = cities[i].y;
        x2 = cities[j].x;
        y2 = cities[j].y;
    }

    // Need to use math.h library for sqrt() and pow(). 
    return sqrt(pow((x2-x1), 2) + pow((y2-y1), 2));
}

int planGrid1(Place cities[], int numCities, Place powerPlant,
              PowerLine powerLines[]) {
    
    // Place is a vertex and powerline is an edge. 
    // Create graph with numCities and place power plant with everything
    // being an edge. 

    // total vertices.
    int nV = numCities + 1; 

    // Create graph with everything connected then find minimum spanning
    // tree from this graph 
    Graph all_edges = GraphNew(nV);
    Edge E; 

    // Make priority queue and add all the posible cities connections
    // then add the power plant connection to those city connections. 

    PQ pq = PQNew();
    // Vertex in edge will be index in cities last vertex is powerplant. 
    for (int i = 0; i < numCities; i++) {
        for (int j = 0; j < numCities; j++) {
            // Prevent duplicates in two ways either being self loops or 
            // the same relationship in other direction as undirected. 
                if (i != j && i < j) {
                    E = ReturnEdgePlanner(i, j, CalculateWeight(i, j, cities, powerPlant, numCities));
                    GraphInsertEdge(all_edges, E);
                    PQInsert(pq, E); 
                }
        }
        E = ReturnEdgePlanner(i, numCities, CalculateWeight(i, numCities, cities, powerPlant, numCities));
        GraphInsertEdge(all_edges, E);
        PQInsert(pq, E);
    }

    int count = 0;
    // Get the minimum spanning tree from graph. 
    Graph MST = GraphMST(all_edges);
    
    // Now we go through all the edges in the edge array and add to the 
    // powerline making sure to tonly add edge once by doing k > j. 

    for (int i = 0; i < nV; i++) {
        for (int j = 0; j < nV; j++) {
            if (MST->edges[i][j] > 0 && i < j) {
                powerLines[count] = ReturnPowerLine(ReturnPlace(i, nV, cities, powerPlant),
                ReturnPlace(j, nV, cities, powerPlant));
                count++;
            }    
        } 
    }
    int number_of_lines = MST->nE;

    // freeing pq, MST and all edges. 
    PQFree(pq);
    GraphFree(MST);
    GraphFree(all_edges);
    
    return number_of_lines;
}

////////////////////////////////////////////////////////////////////////
// Optional task

/**
 * Designs  a minimal cost electrical grid that will deliver electricity
 * to all the given cities.  Power lines must be built between cities or
 * between a city and a power plant.  Cost is directly  proportional  to
 * the  total  length of power lines used.  Assume that each power plant
 * generates enough electricity to supply all cities, so not  all  power
 * plants need to be used.
 * Assumes  that  numCities and numPowerPlants are at least 1 (numCities
 * and numPowerPlants are the sizes of the cities and powerPlants arrays
 * respectively).
 * Stores the power lines that need to be built in the given  powerLines
 * array, and returns the number of power lines stored. Assumes that the
 * powerLines array is large enough to store all required power lines.
 */
int planGrid2(Place cities[], int numCities,
              Place powerPlants[], int numPowerPlants,
              PowerLine powerLines[]) {
    // TODO: Complete this function
    return 0;
}