#pragma once
#include "common.h"

const static int order[4][2] = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};

#define CELL_UNVISITED 0
#define CELL_PATH 1
#define CELL_WASTED 2
#define CELL_START 3
#define CELL_FINISH 4

class Cell {
private:
    int x, y, w, h, val;
    bool visited;
    int neighbourIdx;
    bool walls[4];
    std::vector<std::vector<Cell>>* grid;

public:
    Cell(int _x, int _y, int _w, int _h, int _val, std::vector<std::vector<Cell>>* _grid);
    bool inBounds(int x0, int y0);
    int getVal();
    int getX();
    int getY();
    bool seen();
    void setVal(int _val);
    void visit();
    bool wall(int direction);
    bool north();
    bool east();
    bool south();
    bool west();
    void destroyWall(int direction);
    void reset();
    int directionFromNeighbour(Cell* other);
    void destroyBorder(Cell* other);
    Cell* randomNeighbour();
    std::vector<Cell*> accessibleNeighbours();
};