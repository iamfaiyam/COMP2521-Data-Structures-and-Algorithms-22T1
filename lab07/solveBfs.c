// BFS maze solver
// Faiyam Islam (z5258151)

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Cell.h"
#include "helpers.h"
#include "Maze.h"
#include "Queue.h"

// NONE, VISITED, PATH.
typedef enum {
    NONE,
    VISITED,
    PATH,
} State;

bool isvalid(Maze m, Cell V) {
    int Height = MazeHeight(m), Width = MazeWidth(m); 
    // This statement determines if the cell is within the bounds. 
    if(V.row < Height && V.row >= 0 && V.col < Width && V.col >= 0 && MazeIsWall(m,V) == false) {
        return true;
    }
    else {
        return false;
    }
}

bool adjacent(Maze m, Cell V,Cell W) {
    Cell Up = {V.row + 1, V.col};
    Cell Down = {V.row - 1, V.col};
    Cell Right = {V.row, V.col + 1};
    Cell Left = {V.row, V.col - 1};
    if(isvalid(m,W) == true && isvalid(m,V)) {
        if(isvalid(m, Up) == true) {
            if(Up.row == W.row && Up.col == W.col) {
                return true;
            }
        }
        if(isvalid(m, Down) == true) {
            if(Down.row == W.row && Down.col == W.col) {
                return true;
            }
        }
        if(isvalid(m,Left) == true){
            if(Left.row == W.row && Left.col == W.col) {
                return true;
            }
        }
        if(isvalid(m, Right) == true) {
            if(Right.row == W.row && Right.col == W.col) {
                return true;
            }
        }
    }
        return false;
}

Cell ReturnCell(int row, int col) {
    Cell cell ={.row = row,.col = col};
    return cell;
}

// The solve maze function was implemented from this pseudocode from lectures. 
/*
|  Input  graph G, vertices src,dest
|
|  for all vertices v in G do
|     visited[v]=-1
|  end for
|  found=false
|  visited[src]=src
|  enqueue src into queue Q
|  while not found && Q is not empty do
|  |  dequeue v from Q
|  |  if v=dest then
|  |     found=true
|  |  else
|  |  |  for each (v,w) in edges(G) with visited[w]=-1 do
|  |  |     visited[w]=v
|  |  |     enqueue w into Q
|  |  |  end for
|  |  end if
|  end while
|  if found then
|     display path in dest..src order
|  end if
*/

bool solve(Maze m) {
    bool found = false;
    bool **boolmatrix = createBoolMatrix(MazeHeight(m), MazeWidth(m));
    Cell V;
    Cell W;
    Cell **predecessormatrix = createCellMatrix(MazeHeight(m), MazeWidth(m));
    Cell Start = MazeGetStart(m);

    boolmatrix[Start.row][Start.col] = VISITED;
    Queue Q = QueueNew(); 
    QueueEnqueue(Q, Start);

    while(!QueueIsEmpty(Q)) {
        V = QueueDequeue(Q);
    
        if(MazeVisit(m, V) == true) {
            while(V.row != Start.row || V.col != Start.col) {
                MazeMarkPath(m,V);
                V   = predecessormatrix[V.row][V.col];
            }
            MazeMarkPath(m, V);
            MazeMarkPath(m, Start); 
            found = true;
            return found;
        } else {
            int row = 0; 
            while (row < MazeHeight(m)) {
                int col = 0; 
                while (col < MazeWidth(m)) {

                    W = ReturnCell(row, col);
                    if (boolmatrix[W.row][W.col] == NONE && adjacent(m, V, W) == true) {
                        predecessormatrix[W.row][W.col] = V;
                        MazeVisit(m, W);
                        boolmatrix[W.row][W.col] = VISITED;
                        QueueEnqueue(Q, W);

                    }
                col++;   
                }
            row++; 
            }
        
        }

    }
        freeBoolMatrix(boolmatrix);
        freeCellMatrix(predecessormatrix);
        QueueFree(Q);
        return found;
}