#pragma once
#include "common.h"
#include "Cell.h"
#include "ImageWriter.h"
#include "../deps/gif.h"

class Maze {
private:
    std::vector<std::vector<Cell>> grid;
    int W, H;
    Cell* start, *finish;
    bool solveDFSHelper(Cell* c);
    GifWriter gw;
    ImageWriter iw;

public:
    Maze(int w, int h);

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