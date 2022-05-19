
// DFS maze solver
// Faiyam Islam (z5258151)

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Cell.h"
#include "helpers.h"
#include "Maze.h"
#include "Stack.h"

typedef enum {
    NONE,
    VISITED,
    PATH,
} State;

bool isvalid(Maze m,Cell V) {
    int Height = MazeHeight(m), Width = MazeWidth(m);
    if(V.row < Height && V.row >= 0 && V.col < Width && V.col >= 0 && MazeIsWall(m,V) == false) {
        return true;
    }
    else { 
        return false;
    }

}

bool adjacent(Maze m,Cell V,Cell W) {
    Cell Left = {V.row, V.col - 1};
    Cell Right = {V.row, V.col + 1};
    Cell Up = {V.row + 1,V.col};
    Cell Down = {V.row - 1,V.col};
    if(isvalid(m,W) == true && isvalid(m,V)) {
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
    }
        return false;
}

Cell ReturnCell(int row, int col) {
    Cell cell ={.row = row,.col = col};
    return cell;
}

// The following function to solve DFS non-recursively is implemented from 
// the following DFS pseudocode from the lecture: 
/*
visited[] // store previously visited node // for each vertex 0..nV-1
findPathDFS(G,src,dest):
| Input graph G, vertices src,dest
|
| for all vertices v∈G do
| visited[v]=-1
| end for
| found=false
| visited[src]=src
| push src onto new stack S
| while not found ∧ S is not empty do | | popvfromS
| | if v=dest then
| | found=true
| | else
  | | |
| | |
| | |
| | | end for | | endif
for each (v,w)∈edges(G) with visited[w]=-1 do visited[w]=v
push w onto S
| end while
| if found then
| display path in dest..src order | end if */

bool dfsPathCheck(Maze m,Cell V,Cell **predecessormatrix,bool **boolmatrix) {
    MazeVisit(m,V);
    boolmatrix[V.row][V.col] = VISITED;
    Cell W;
    int row = 0; 
    while (row < MazeHeight(m)) {
            int col = 0; 
            while (col < MazeWidth(m)) {
                
                W = ReturnCell(row,col);
                if (adjacent(m, V, W) == true) {

                    if (MazeVisit(m,W) == true) {
                        MazeMarkPath(m, W);
                        predecessormatrix[W.row][W.col] = V;
                        Cell Start = MazeGetStart(m);
                        while(W.row != Start.row || W.col != Start.col) {
                            MazeMarkPath(m, W);
                            W = predecessormatrix[W.row][W.col];
                        }

                        MazeMarkPath(m, Start); 
                        return true;
                    } else if (boolmatrix[W.row][W.col] == NONE) {
                        predecessormatrix[W.row][W.col] = V;
                        if(dfsPathCheck(m, W, predecessormatrix, boolmatrix) == true) {
                            return true;
                        }
                    }

                }
            col++;
            }
    row++;
    }

    return false;
}

bool solve(Maze m) {
    bool found = false;
    bool **boolmatrix = createBoolMatrix(MazeHeight(m), MazeWidth(m));
    Cell **predecessormatrix = createCellMatrix(MazeHeight(m), MazeWidth(m));
    Cell Start = MazeGetStart(m);
    found = dfsPathCheck(m, Start, predecessormatrix, boolmatrix);
    freeCellMatrix(predecessormatrix);
    freeBoolMatrix(boolmatrix);
    return found;
}
