#pragma once
#include "common.h"

typedef uint uint32_t;
typedef unsigned char uchar;
typedef uchar mnode;
typedef std::pair<int, int> coord;
typedef std::vector<mnode*> mnode_vec;
typedef std::pair<mnode*, mnode*> mnode_edge;

// first four bits are state flags, last 4 flags are the states of the walls
#define NORTH 0
#define EAST 1
#define SOUTH 2
#define WEST 3
#define OPPOSITE_DIRECTION(X) (X+2%4)
#define MNODE_CLEAN                0b00001111
#define MNODE_NOWALL               0b00000000
#define MNODE_GET_WALL(X, Y) (X & (1 << (3-Y)))
#define MNODE_REMOVE_WALL(X, Y) (X &= ~(1 << (3-Y)))
#define MNODE_VISIT(X)       (X |= 0b10000000)
#define MNODE_VISITED(X)     (X &  0b10000000)
#define MNODE_UNVISIT(X)     (X &= 0b01111111)
#define MNODE_SET_PATH(X)    (X |= 0b01000000)
#define MNODE_PATH(X)        (X &  0b01000000)
#define MNODE_REMOVE_PATH(X) (X &= 0b10111111)
#define MNODE_SET_WASTED(X)  (X |= 0b00100000)
#define MNODE_WASTED(X)      (X &  0b00100000)

#define SHUFFLE(X) std::shuffle(X.begin(), X.end(), std::default_random_engine{seed})

static const coord DIRECTIONS[4] {{0,-1},{1,0},{0,1},{-1,0}};

template <typename T,typename U>                                                   
std::pair<T,U> operator+(const std::pair<T,U> & l,const std::pair<T,U> & r) {   
    return {l.first+r.first,l.second+r.second};                                    
}     

class Maze {
private:
    mnode* data, *start, *finish;
    uint seed;
    uint W, H;
    uint area;
    uint stride;
    
    inline bool inBounds(const uint x, const uint y);
    inline bool inBounds(const coord& c);
    
    bool solveDFSHelper(mnode* c, int& steps);
    void dfsGenHelper(mnode* c);
    double distCell(mnode* a, mnode* b);
    mnode* setFind(std::unordered_map<mnode*, mnode*>& s, mnode* c);
    void setUnion(std::unordered_map<mnode*, mnode*>& s, mnode* a, mnode* b);
    void addMst(mnode* c, int idx, std::deque<mnode*>& frontier, std::unordered_set<mnode*>& fset, std::unordered_set<mnode*>& mst);
    void addFrontier(mnode* c, std::deque<mnode*>& frontier, std::unordered_set<mnode*>& fset);

    void removeEdge(mnode* a, mnode* b);
    void removeEdge(mnode_edge& e);
    inline mnode_vec allNeighbours(mnode* m);
    inline mnode_vec visitedNeighbours(mnode* m);
    inline mnode_vec unvisitedNeighbours(mnode* m);
    inline mnode_vec accessibleNeighbours(mnode* m);


    
public:
    Maze(uint w, uint h);
    inline void reset();

    inline void resize(uint newW, uint newH);
    inline void unsolve();

    inline uint getWidth();
    inline uint getHeight();
    inline mnode* getNode(const coord& c);
    inline coord getCoord(mnode* m);
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