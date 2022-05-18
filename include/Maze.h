#pragma once
#include "common.h"
#include "Cell.h"
#include "ImageWriter.h"

#define DEFAULT_WIDTH 20
#define DEFAULT_HEIGHT 20
#define DEFAULT_CELL_LEN 8
#define DEFAULT_WALL_LEN 2
#define DEFAULT_GIF_DELAY 20

typedef struct
{
    FILE* f;
    uint8_t* oldImage;
    bool firstFrame;
} GifWriter;

class Maze {
private:
    std::vector<std::vector<Cell>> grid;
    int W, H, cellLen, wallLen;
    Cell* start, *finish;
    const char* dir;
    GifWriter* gw;
    int gifDelay;
    bool saveGif;
    ImageWriter* iw;

    bool solveDFSHelper(Cell* c);
    void dfsGenHelper(Cell* c);
    
public:
    Maze(int w = DEFAULT_WIDTH, int h = DEFAULT_HEIGHT, int _wallLen = DEFAULT_WALL_LEN, int _cellLen = DEFAULT_CELL_LEN);

    int getVal(int x, int y);
    int getWidth();
    int getHeight();
    Cell* getCell(int x, int y);

    void genDFS();
    void genKruskal();
    
    void solveDFS();
    void solveBFS();
    void solveAStar();
    void solveDijkstra();

    void startGif(const char* gifName);
    void updateGif();
    void endGif();

    void updateImage();
    void saveImage(const char* imgName);

};