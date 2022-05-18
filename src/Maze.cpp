#include "../include/Maze.h"

Maze::Maze(int w, int h) {
    W = w;
    H = h;
    for (int i = 0; i < w; ++i) {
        grid.push_back({});
        for (int j = 0; j < h; ++j) {
            grid[i].push_back({i, j, w, h, 0, &grid});
        }
    }
    start = &grid[W/2][0];
    finish = &grid[W/2][H-1];
}

int Maze::getVal(int x, int y) {
    return grid[x][y].getVal();
}

int Maze::getHeight() {
    return H;
}

int Maze::getWidth() {
    return W;
}

Cell* Maze::getCell(int x, int y) {
    return &grid[x][y];
}

/* -------------------- GENERATORS -------------------- */

void dfsGenHelper(Cell* c) {
    if (c->seen()) return;
    c->visit();
    Cell* nextCell = c->randomNeighbour();
    while (nextCell != nullptr) {
        c->destroyBorder(nextCell);
        dfsGenHelper(nextCell);
        nextCell = c->randomNeighbour();
    }
}

void Maze::genDFS() {
    dfsGenHelper(start);
    start->destroyWall(0);
    finish->destroyWall(2);
}

void Maze::genUnion() {

}
void Maze::genKhruskhal() {

}

/* -------------------- SOLVERS -------------------- */

bool Maze::solveDFSHelper(Cell* c) {
    if (c == finish) {
        c->setVal(CELL_PATH);
        return true;
    }
    if (c->getVal() == 1) return false;
    c->setVal(CELL_PATH);
    std::vector<Cell*> accessible = c->accessibleNeighbours();
    for (auto n : accessible) 
        if (solveDFSHelper(n)) return true;
    c->setVal(CELL_WASTED);
    return false;
}

void Maze::solveDFS() {
   solveDFSHelper(start);
   start->setVal(CELL_START);
   finish->setVal(CELL_FINISH);
} 

void Maze::solveBFS() {

}
void Maze::solveAStar() {

}
void Maze::solveDijkstra() {

}