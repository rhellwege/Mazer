#include "Maze.h"
#include "../deps/gif.h"

Maze::Maze(uint w, uint h) {
    W = w;
    H = h;
    data = (mnode*)malloc(W*H*sizeof(mnode));
    reset();
    start = data;
    finish = start + (W*H*sizeof(mnode));
}

inline void Maze::reset() { memset(data, MNODE_CLEAN, sizeof(data)); }

inline mnode* Maze::getNode(uint x, uint y) {

}

void Maze::resize(uint newW, uint newH) {
    W = newW;
    H = newH;
    free(data);
    (mnode*)malloc(W*H*sizeof(mnode));
    reset();
    start = data;
    finish = start + (W*H*sizeof(mnode));
}

void Maze::unsolve() {
    for (uint i = 0; i < sizeof(data); ++i) {
        data[i] &= 0b00001111;
    }
}

inline uint Maze::getHeight() { return H; }
inline uint Maze::getWidth() { return W; }
inline uint Maze::getSeed() { return seed; }
inline void Maze::setSeed(uint newSeed) { srand(newSeed); seed = newSeed; }

/* -------------------- GENERATORS -------------------- */
void Maze::dfsGenHelper(mnode* c) {
    Cell* nextCell = c->randomNeighbour();
    while (nextCell != nullptr) {
        c->destroyBorder(nextCell);
        updateBorderPixels(c, c->directionFromNeighbour(nextCell), COLOR_WHITE);
        if (saveGif) {
            addFrame();
        }
        dfsGenHelper(nextCell);
        nextCell = c->randomNeighbour();
    }
}

void Maze::genDFS() {
    if (saveGif) startGif((dir + "gen(dfs).gif").c_str());
    dfsGenHelper(&grid[rand()%W][rand()%H]);
    if (!saveGif)
        updateImage();
    if (saveGif) endGif();
}

Cell* Maze::setFind(std::unordered_map<Cell*, Cell*>& s, Cell* c) {
    if (s[c] == c) return c;
    return setFind(s, s[c]);
}

void Maze::setUnion(std::unordered_map<Cell*, Cell*>& s, Cell* a, Cell* b) {
    Cell* aParent = setFind(s, a);
    Cell* bParent = setFind(s, b);
    s[aParent] = bParent;
}

void Maze::genKruskal() {
    if (saveGif) startGif((dir + "gen(kruskal).gif").c_str());
    std::vector<std::pair<Cell*, Cell*>> edges;
    std::unordered_map<Cell*, Cell*> sets;
    int wallsDown = 0;
    for (int i = 0; i < W ; ++i) {
        for (int j = 0; j < H; ++j) {
            Cell* c = getCell(i, j);
            sets[c] = c;
            if (i < W - 1)
                edges.push_back(std::make_pair(c, getCell(i + 1, j)));
            if (j < H - 1)
                edges.push_back(std::make_pair(c, getCell(i, j+1)));
        }
    }
    //shuffle the edges:
    std::shuffle(edges.begin(), edges.end(), std::default_random_engine{seed});
    // kruskal
    while (edges.size() && wallsDown < W*H-1) {
        std::pair<Cell*, Cell*> cur = edges[edges.size()-1]; edges.pop_back();
        if (setFind(sets, cur.first) != setFind(sets, cur.second)) {
            setUnion(sets, cur.first, cur.second); // union the sets
            cur.first->destroyBorder(cur.second);
            updateBorderPixels(cur.first, cur.first->directionFromNeighbour(cur.second), COLOR_WHITE);
            if (saveGif) addFrame();
            wallsDown++;
        }
    }
    if (!saveGif)
        updateImage();
    if (saveGif) endGif();
}

void addFrontier(Cell* c, std::deque<Cell*>& frontier, std::unordered_set<Cell*>& fset) {
    if (fset.find(c) != fset.end()) {
        printf("already in fset.\n");
        return;
    }
    frontier.push_back(c);
    fset.insert(c);
}

void addMst(Cell* c, int idx, std::deque<Cell*>& frontier, std::unordered_set<Cell*>& fset, std::unordered_set<Cell*>& mst) {
    if (mst.find(c) != mst.end()) return;
    if (fset.find(c) != fset.end())
        fset.erase(c);
    if (idx != -1) {
        frontier.erase(frontier.begin() + idx);
    }
    mst.insert(c);
    std::vector<Cell*> neighbours = c->unvisitedNeighbours();
    for (auto n : neighbours) {
        if (fset.find(n) == fset.end()) {
            addFrontier(n, frontier, fset);
        }
    }

}

void Maze::genPrims() {
    if (saveGif) startGif((dir + "gen(prims).gif").c_str());
    std::unordered_set<Cell*> mst; // minimal spanning tree
    std::deque<Cell*> frontier; // all cells adjacent to the mst
    std::unordered_set<Cell*> fset;
    Cell* a = &grid[rand() % W][rand() % H];
    addMst(a, -1, frontier, fset, mst);
    a->visit();
    while (frontier.size()) {
        // choose random frontier
        int idx = rand() % frontier.size();
        Cell* c = frontier[idx];
        addMst(c, idx, frontier, fset, mst);
        // destroy the walls of one of the adjacent mst nodes (at random)
        c->visit();
        std::vector<Cell*> ins = c->visitedNeighbours();
        Cell* neighbour = ins[rand() % ins.size()];
        c->destroyBorder(neighbour);
        updateBorderPixels(c, c->directionFromNeighbour(neighbour), COLOR_WHITE);
        if (saveGif) addFrame();
    }
    if (!saveGif)
        updateImage();
    if (saveGif) endGif();
}

/* -------------------- SOLVERS -------------------- */
bool Maze::solveDFSHelper(Cell* c, int& steps) {
    ++steps;
    if (c == finish) return true;
    if (c->getVal() == CELL_PATH) return false;
    if (c != start)
        c->setVal(CELL_PATH);
    if (saveGif) {
        updateCellCol(c);
        addFrame();
    }
    std::vector<Cell*> accessible = c->accessibleNeighbours();
    for (auto n : accessible) 
        if (solveDFSHelper(n, steps)) return true;
    if (c != start)
        c->setVal(CELL_WASTED);
    if (saveGif) {
        updateCellCol(c);
        addFrame();
    }
    return false;
}

int Maze::solveDFS() {
    solveDFSHelper(start, steps);
    if (!saveGif) updateImage();
    if (saveGif) endGif();
    return steps;
} 

int Maze::solveBFS() {
    int steps;
    if (saveGif) startGif((dir +"solve(BFS).gif").c_str());
    std::queue<Cell*> q;
    std::unordered_map<Cell*, Cell*> path;
    Cell* current = start;
    while (current != finish) {
        
        if (current != start)
            current->setVal(CELL_WASTED);
        ++steps;
        if (saveGif) {
            updateCellCol(current);
            addFrame();
        }
        std::vector<Cell*> neighbours = current->accessibleNeighbours();
        for (auto neighbour : neighbours) {
            if (neighbour->getVal() != CELL_WASTED)  {
                q.push(neighbour);
                path[neighbour] = current;
            }
        }
        current = q.front(); q.pop();
    }
    while (current != start) {
        current = path[current];
        if (current == start) break;
        current->setVal(CELL_PATH);
        if (saveGif) {
            updateCellCol(current);
            addFrame();
        }
    }
    if (!saveGif) updateImage();
    if (saveGif) endGif();
    return steps;
}

double Maze::distCell(mnode* a, mnode* b) {
    int w = b->getX() - a->getX();
    int h = b->getY() - a->getY();
    return sqrt(x*x + y*y);
}

void Maze::solveAStar() {
    std::unordered_map<Cell*, double> cost;
    std::unordered_map<Cell*, Cell*> prev;
    std::priority_queue<std::pair<double, Cell*>, std::vector<std::pair<double, Cell*>>, std::greater<std::pair<double, Cell*>>> pq;
    cost[start] = 0 + distCell(start, finish);
    for (int i = 0; i < W ; ++i) {
        for (int j = 0; j < H; ++j) {
            Cell* c = getCell(i, j);
            if (c != start) {
                cost[c] = DBL_MAX;
            }
        }
    }
    pq.push(std::make_pair(0, start));
    while (!pq.empty()) {
        Cell* u = pq.top().second;
        if (u == finish) {
            break;
        }
        if (u != start)
            u->setVal(CELL_WASTED);
        ++steps;
        if (saveGif) {
            updateCellCol(u);
            addFrame();
        }
        pq.pop();
        std::vector<Cell*> neighbours = u->accessibleNeighbours();
        for (auto v : neighbours) {
            if (cost[v] > cost[u] + distCell(u, finish)) {
                cost[v] = cost[u] + distCell(u, finish);
                prev[v] = u;
                pq.push(std::make_pair(cost[v], v));
            }
        }
    }
    // color path
    Cell* cur = prev[finish];
    while (cur != start) {
        cur->setVal(CELL_PATH);
        if (saveGif) {
            updateCellCol(cur);
            addFrame();
        }
        cur = prev[cur];
    }
    if (!saveGif) updateImage();
    if (saveGif) endGif();
    return steps;
}

void Maze::solveDijkstra() {
    std::unordered_map<Cell*, unsigned int> distance;
    std::unordered_map<Cell*, Cell*> prev;
    std::priority_queue<std::pair<unsigned int, Cell*>, std::vector<std::pair<unsigned int, Cell*>>, std::greater<std::pair<unsigned int, Cell*>>> pq;
    distance[start] = 0;
    for (int i = 0; i < W ; ++i) {
        for (int j = 0; j < H; ++j) {
            Cell* c = getCell(i, j);
            if (c != start) {
                distance[c] = INT_MAX;
            }
        }
    }
    pq.push(std::make_pair(0, start));
    while (!pq.empty()) {
        Cell* u = pq.top().second;
        if (u == finish) {
            break;
        }
        if (u != start)
            u->setVal(CELL_WASTED);
        if (saveGif) {
            updateCellCol(u);
            addFrame();
        }
        ++steps;
        pq.pop();
        std::vector<Cell*> neighbours = u->accessibleNeighbours();
        for (auto v : neighbours) {
            
            if (distance[v] > distance[u] + 1) {
                distance[v] = distance[u] + 1;
                prev[v] = u;
                pq.push(std::make_pair(distance[v], v));
            }
        }
    }
    // color path
    Cell* cur = prev[finish];
    while (cur != start) {
        cur->setVal(CELL_PATH);
        if (saveGif) {
            updateCellCol(cur);
            addFrame();
        }
        cur = prev[cur];
    }
    if (!saveGif) updateImage();
    if (saveGif) endGif();
    return steps;
}