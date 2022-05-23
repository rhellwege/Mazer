#include "../include/Cell.h"

Cell::Cell(int _x, int _y, int _w, int _h, int _val, std::vector<std::vector<Cell>>* _grid) {
    x = _x;
    y = _y;
    w = _w;
    h = _h;
    val = _val;
    grid = _grid;
    visited = false;
    for (int i = 0; i < 4; ++i) walls[i] = true;
}

bool Cell::inBounds(int x0, int y0) {
    return (x0 >= 0 && x0 < w && y0 >= 0 && y0 < h);
}

bool Cell::wall(int direction) {
    if (direction >= 4 || direction < 0) return false;
    return walls[direction];
}

int Cell::getVal() {
    return val;
}

int Cell::getX() {
    return x;
}

int Cell::getY() {
    return y;
}

bool Cell::seen() {
    return visited;
}

void Cell::setVal(int _val) {
    val = _val;
}

void Cell::visit() {
    visited = true;
}

bool Cell::north() {
    return walls[0];
}

bool Cell::east() {
    return walls[1];
}

bool Cell::south() {
    return walls[2];
}

bool Cell::west() {
    return walls[3];
}

void Cell::destroyWall(int direction) {
    if (direction >= 4 || direction < 0) return;
    walls[direction] = false;
}

void Cell::reset() {
    for (int i = 0; i < 4; ++i) walls[i] = true;
    val = 0;
    visited = false;
}

int Cell::directionFromNeighbour(Cell* other) {
    if (other->y - y == -1) return 0;
    if (other->x - x == 1) return 1;
    if (other->y - y == 1) return 2;
    if (other->x - x == -1) return 3;
    return -1;
}

void Cell::destroyBorder(Cell* other) {
    int direction = this->directionFromNeighbour(other);
    destroyWall(direction);
    other->destroyWall((direction + 2)%4);
}

Cell* Cell::randomNeighbour() {
    std::vector<Cell*> v;
    for (int i = 0; i < 4; ++i) {
        int newX = x+order[i][0];
        int newY = y+order[i][1];
        if (inBounds(newX, newY) && !(*grid)[newX][newY].seen())
            v.push_back(&(*grid)[newX][newY]);
    }
    if (v.size() == 0) return nullptr; // no more neighbours, please backtrack
    return v[rand()%v.size()];
}

std::vector<Cell*> Cell::accessibleNeighbours() {
    std::vector<Cell*> v;
    for (int i = 0; i < 4; ++i) {
        int newX = x+order[i][0];
        int newY = y+order[i][1];
        if (inBounds(newX, newY) && !walls[i])
            v.push_back(&(*grid)[newX][newY]);
    }
    return v;
}

std::vector<Cell*> Cell::allNeighbours() {
    std::vector<Cell*> v;
    for (int i = 0; i < 4; ++i) {
        int newX = x+order[i][0];
        int newY = y+order[i][1];
        if (inBounds(newX, newY))
            v.push_back(&(*grid)[newX][newY]);
    }
    return v;
}

std::vector<Cell*> Cell::unvisitedNeighbours() {
    std::vector<Cell*> v;
    for (int i = 0; i < 4; ++i) {
        int newX = x+order[i][0];
        int newY = y+order[i][1];
        if (inBounds(newX, newY) && !(*grid)[newX][newY].seen())
            v.push_back(&(*grid)[newX][newY]);
    }
    return v;
}

std::vector<Cell*> Cell::visitedNeighbours() {
    std::vector<Cell*> v;
    for (int i = 0; i < 4; ++i) {
        int newX = x+order[i][0];
        int newY = y+order[i][1];
        if (inBounds(newX, newY) && (*grid)[newX][newY].seen())
            v.push_back(&(*grid)[newX][newY]);
    }
    return v;
}