#pragma once
#include "common.h"
#include "imgui.h"

using namespace std;

typedef uint32_t uint;
typedef unsigned char uchar;
typedef uchar mnode;
typedef pair<int, int> coord;
typedef vector<uint> mnode_vec;
typedef pair<uint, uint> mnode_edge;

// first four bits are state flags, last 4 flags are the states of the walls
#define NORTH 0
#define EAST 1
#define SOUTH 2
#define WEST 3
#define OPPOSITE_DIRECTION(X) (((X)+2)%4)
#define MNODE_CLEAN                0b00001111
#define MNODE_NOWALL               0b00000000
#define MNODE_GET_WALL(X, Y) ((X) & (1 << (3-(Y))))
#define MNODE_REMOVE_WALL(X, Y) ((X) &= ~(1 << (3-(Y))))
#define MNODE_VISIT(X)       ((X) |= 0b10000000)
#define MNODE_VISITED(X)     ((X) &  0b10000000)
#define MNODE_UNVISIT(X)     ((X) &= 0b01111111)
#define MNODE_SET_PATH(X)    ((X) |= 0b01000000)
#define MNODE_PATH(X)        ((X) &  0b01000000)
#define MNODE_REMOVE_PATH(X) ((X) &= 0b10111111)
#define MNODE_SET_WASTED(X)  ((X) |= 0b00100000)
#define MNODE_WASTED(X)      ((X) &  0b00100000)
#define MNODE_START(X)       ((X) &  0b00010000)
#define MNODE_SET_START(X)   ((X) |= 0b00010000)
#define MNODE_FINISH(X)      (((X) &  0b01110000) == 0b01110000)
#define MNODE_SET_FINISH(X)  ((X) |= 0b01110000)
#define MNODE_ACTIVE(X)      ((X) == -1)

#define SHUFFLE(X) shuffle((X).begin(), (X).end(), default_random_engine{seed})

static const coord DIRECTIONS[4] {
    make_pair(0,-1),
    make_pair(1,0),
    make_pair(0,1),
    make_pair(-1,0)
};

class Maze {
private:
    mnode* data;
    mnode* generatedData;
    uint start, finish;
    uint activeNode;
    uint seed;
    uint W, H;
    uint area;
    uint stride;
    vector<pair<uint, mnode>> history; // stores history of every change to the maze. The unsolved state will always be the number of cells

    bool generated;
    bool solved;

    bool inBounds(const uint x, const uint y);
    bool inBounds(const coord& c);

    void solveDFSHelper(uint c, uint& steps, uint& pathLen);
    void dfsGenHelper(uint c, uint& steps);
    double distCell(uint a, uint b);
    uint setFind(unordered_map<uint, uint>& s, uint c);
    void setUnion(unordered_map<uint, uint>& s, uint a, uint b);
    void addMst(uint c, int idx, deque<uint>& frontier, unordered_set<uint>& fset, unordered_set<uint>& mst);
    void addFrontier(uint c, deque<uint>& frontier, unordered_set<uint>& fset);

    void removeEdge(uint a, uint b);
    void removeEdge(mnode_edge& e);
    mnode_vec allNeighbours(uint m);
    mnode_vec visitedNeighbours(uint m);
    mnode_vec unvisitedNeighbours(uint m);
    mnode_vec accessibleNeighbours(uint m);
    uint randomUnvisited(uint m);

public:
    ImVec2 canvas_p0, canvas_sz, wall_sz, cell_sz, full_sz;
    int delay;
    bool loopAnimation;
    uint animationStep;
    float cell_to_wall;
    bool animate;

    void stopAnimation();

    Maze(uint w, uint h);
    void reset();

    void resize(uint newW, uint newH);
    void unsolve();

    bool isGenerated();
    bool isSolved();
    uint getWidth();
    uint getHeight();
    uint getArea();
    uint getNode(const coord& c);
    uint getNode(const uint x, const uint y);
    coord getCoord(uint m);
    uint getSeed();
    void setSeed(uint newSeed);
    void resetSeed();

    void genDFS(uint& steps);
    void genKruskal(uint& steps);
    void genPrims(uint& steps);

    // solvers return the amount of steps taken until it found the end (this includes the back tracking)
    void solveDFS(uint& steps, uint& pathLen);
    void solveBFS(uint& steps, uint& pathLen);
    void solveAStar(uint& steps, uint& pathLen);
    void solveDijkstra(uint& steps, uint& pathLen);

    void generate(const string& funcName, uint& steps);
    void solve(const string& funcName, uint& steps, uint& pathLen);

    void display();
    // for ray casting:
    bool isWall(ImVec2 pos); // samples actual screen pixels
    ImU32 getFillCol(uint m);

    uint floatToPixel(ImVec2 c);
    float pixelToFloat();
};
