#pragma once
#include "common.h"
#include "Cell.h"
#include "ImageWriter.h"
#include "../deps/gif.h"

#define DEFAULT_CELL_LEN 8
#define DEFAULT_WALL_LEN 2

class Maze {
private:
    std::vector<std::vector<Cell>> grid;
    int W, H, cellLen, wallLen;
    Cell* start, *finish;
    bool solveDFSHelper(Cell* c);
    GifWriter gw;
    ImageWriter iw;

public:
    Maze(int w, int h, int _wallLen = DEFAULT_WALL_LEN, int _cellLen = DEFAULT_CELL_LEN);

    int getVal(int x, int y);
    int getWidth();
    int getHeight();
    Cell* getCell(int x, int y);

    void genDFS();
    void genUnion();
    void genKhruskhal();
    
    void solveDFS();
    void solveBFS();
    void solveAStar();
    void solveDijkstra();

    void startGif(const char* gifName);
    void endGif();

    void updateImage();
    void saveImage(const char* imgName);

};