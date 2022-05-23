#pragma once
#include "common.h"
#include "Cell.h"
#include "ImageWriter.h"

typedef struct
{
    FILE* f;
    uint8_t* oldImage;
    bool firstFrame;
} GifWriter;

class Maze {
private:
    std::vector<std::vector<Cell>> grid;
    unsigned int seed;
    int W, H, cellLen, wallLen;
    Cell* start, *finish;
    std::string dir;
    GifWriter* gw;
    int gifDelay;
    bool saveGif;
    ImageWriter* iw;

    bool solveDFSHelper(Cell* c);
    void dfsGenHelper(Cell* c);

    Cell* setFind(std::unordered_map<Cell*, Cell*>& s, Cell* c);
    void setUnion(std::unordered_map<Cell*, Cell*>& s, Cell* a, Cell* b);
    
public:
    Maze(int w , int h, int _cellLen, int _wallLen, bool _saveGif, int _gifDelay, unsigned int _seed, std::string _dir);

    int getVal(int x, int y);
    int getWidth();
    int getHeight();
    double distCell(Cell* a, Cell* b);
    Cell* getCell(int x, int y);

    void genDFS();
    void genKruskal();
    void genPrims();
    
    void solveDFS();
    void solveBFS();
    void solveAStar();
    void solveDijkstra();

    void startGif(const char* gifName);
    void updateGif();
    void addFrame();
    void endGif();

    void imgSetup();
    void updateImage();
    void updateCellCol(Cell* c);
    void updateBorderPixels(Cell* c, int direction, RGBA col);
    void updateCellPixels(Cell* c);
    void saveImage(const char* imgName);
};