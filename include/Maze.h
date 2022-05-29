#pragma once
#include "common.h"

typedef uint uint32_t;
typedef unsigned char uchar;
typedef uchar mnode;
typedef std::pair<uint, uint> coord;
typedef std::vector<mnode*> mnode_vec;

// first four bits are state flags, last 4 flags are the states of the walls
#define MNODE_CLEAN           0b00001111
#define MNODE_NOWALL          0b00000000
#define MNODE_NORTH(X)   (X & 0b00001000)
#define MNODE_EAST(X)    (X & 0b00000100)
#define MNODE_SOUTH(X)   (X & 0b00000010)
#define MNODE_WEST(X)    (X & 0b00000001)
#define MNODE_VISITED(X) (X & 0b10000000)
#define MNODE_PATH(X)    (X & 0b01000000)
#define MNODE_WASTED(X)  (X & 0b00100000)

class Maze {
private:
    mnode* data, *start, *finish;
    uint seed;
    uint W, H;
    
    bool solveDFSHelper(mnode* c, int& steps);
    void dfsGenHelper(mnode* c);
    double distCell(mnode* a, mnode* b);
    mnode* setFind(std::unordered_map<mnode*, mnode*>& s, mnode* c);
    void setUnion(std::unordered_map<mnode*, mnode*>& s, mnode* a, mnode* b);

    inline mnode_vec allNeighbours();
    inline mnode_vec visitedNeighbours();
    inline mnode_vec unvisitedNeighbours();
    
public:
    Maze(uint w, uint h);
    inline void reset();

    void resize(uint newW, uint newH);
    void unsolve();

    inline uint getWidth();
    inline uint getHeight();
    inline mnode* getNode(uint x, uint y);
    inline coord getCoord(mnode*);
    inline uint getSeed();
    inline void setSeed(uint newSeed);
    
    void genDFS();
    void genKruskal();
    void genPrims();
    
    // solvers return the amount of steps taken until it found the end (this includes the back tracking)
    void solveDFS(uint& steps, uint& pathLen);
    void solveBFS(uint& steps, uint& pathLen);
    void solveAStar(uint& steps, uint& pathLen);
    void solveDijkstra(uint& steps, uint& pathLen);
};