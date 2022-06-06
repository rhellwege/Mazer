#include "../include/Maze.h"

static std::unordered_map<std::string, void (Maze::*)(uint&)> GEN_DICT = {
    {"DFS", &Maze::genDFS},
    {"Kruskal", &Maze::genKruskal},
    {"Prims", &Maze::genPrims},
    {"Prim's", &Maze::genPrims},
};

static std::unordered_map<std::string, void (Maze::*)(uint&, uint&)> SOLVE_DICT = {
    {"DFS", &Maze::solveDFS},
    {"BFS", &Maze::solveBFS},
    {"Dijkstra", &Maze::solveDijkstra},
    {"A*", &Maze::solveAStar}
};

Maze::Maze(uint w, uint h) {
    generated = false;
    solved = false;
    resize(w, h);
    seed = time(NULL);
}

void Maze::reset() {
    memset(data, MNODE_CLEAN, area); generated = false; solved = false;
    start = data;
    finish = start + area - 1;
    MNODE_SET_START(*data);
    MNODE_SET_FINISH(*finish); 
}
bool Maze::inBounds(uint x, uint y) { return !(x < 0 || y < 0 || x >= W || y >= H);}
bool Maze::inBounds(const coord& c) { return inBounds(c.first, c.second); }
bool Maze::isGenerated() {return generated;}
bool Maze::isSolved() {return solved;}
mnode* Maze::getNode(const coord& c) { return getNode(c.first, c.second); }
mnode* Maze::getNode(const uint x, const uint y) {
    mnode* node = (mnode*)(data + y * stride + x);
    return inBounds(x, y) ? node : nullptr;
}
coord Maze::getCoord(mnode* m) {
    uint pos = m - data;
    return std::make_pair(pos % stride, pos / stride);
}
void Maze::removeEdge(mnode* a, mnode* b) {
    coord aCoord = getCoord(a);
    coord bCoord = getCoord(b);
    coord diff  = bCoord - aCoord;
    for (int i = 0; i < 4; ++i) {
        if (diff == DIRECTIONS[i]) {
            MNODE_REMOVE_WALL(*a, i);
            MNODE_REMOVE_WALL(*b, (i+2)%4);
        }
    }
}
void Maze::removeEdge(mnode_edge& e) {
    removeEdge(e.first, e.second);
}
void Maze::resize(uint newW, uint newH) {
    W = newW;
    H = newH;
    area = W*H;
    stride = W * sizeof(mnode);
    if (data)
        free(data);
    data = (mnode*)malloc(area*sizeof(mnode));
    reset();
    
}
void Maze::unsolve() {
    for (uint i = 0; i < area; ++i) {
        data[i] &= 0b00001111;
    }
    solved = false;
    MNODE_SET_START(*start);
    MNODE_SET_FINISH(*finish);
}
uint Maze::getHeight() { return H; }
uint Maze::getWidth() { return W; }
uint Maze::getArea() { return area; }
uint Maze::getSeed() { return seed; }
void Maze::setSeed(uint newSeed) { srand(newSeed); seed = newSeed; }

mnode_vec Maze::allNeighbours(mnode* m) {
    mnode_vec v;
    coord c = getCoord(m);
    for (int i = 0; i < 4; ++i) {
        coord newCoord = c + DIRECTIONS[i];
        if (inBounds(newCoord)) {
            v.push_back(getNode(newCoord));
        }
    }
    return v;
}
mnode_vec Maze::visitedNeighbours(mnode* m) {
    mnode_vec v;
    coord c = getCoord(m);
    for (int i = 0; i < 4; ++i) {
        coord newCoord = c + DIRECTIONS[i];
        if (inBounds(newCoord) ) {
            mnode* newNode = getNode(newCoord);
            if (MNODE_VISITED(*(getNode(newCoord))))
                v.push_back(newNode);
        }
    }
    return v;
}
mnode_vec Maze::unvisitedNeighbours(mnode* m) {
    mnode_vec v;
    coord c = getCoord(m);
    for (int i = 0; i < 4; ++i) {
        coord newCoord = c + DIRECTIONS[i];
        if (inBounds(newCoord)) {
            mnode* newNode = getNode(newCoord);
            if (!MNODE_VISITED(*(getNode(newCoord))))
                v.push_back(newNode);
        }
    }
    return v;
}

mnode_vec Maze::accessibleNeighbours(mnode* m) {
    mnode_vec v;
    coord c = getCoord(m);
    for (int i = 0; i < 4; ++i) {
        coord newCoord = c + DIRECTIONS[i];
        if (inBounds(newCoord)) {
            mnode* newNode = getNode(newCoord);
            if (!MNODE_GET_WALL(*newNode, i))
                v.push_back(newNode);
        }
    }
    return v;
}

/* -------------------- GENERATORS -------------------- */
void Maze::dfsGenHelper(mnode* c, uint& steps) {
    if (MNODE_VISITED(*c)) return;
    MNODE_VISIT(*c);
    ++steps;
    std::this_thread::sleep_for(std::chrono::milliseconds(delay));
    mnode_vec neighbours = unvisitedNeighbours(c);
    mnode* n = neighbours[rand()%neighbours.size()];
    if (!MNODE_VISITED(*n)) {
        removeEdge(c, n);
        dfsGenHelper(n, steps);
    }
}

void Maze::genDFS(uint& steps) {
    dfsGenHelper(data + (rand() % area), steps);
    generated = true;
}

mnode* Maze::setFind(std::unordered_map<mnode*, mnode*>& s, mnode* c) {
    if (s[c] == c) return c;
    return setFind(s, s[c]);
}

void Maze::setUnion(std::unordered_map<mnode*, mnode*>& s, mnode* a, mnode* b) {
    mnode* aParent = setFind(s, a);
    mnode* bParent = setFind(s, b);
    s[aParent] = bParent;
}

void Maze::genKruskal(uint& steps) {
    std::vector<mnode_edge> edges;
    std::unordered_map<mnode*, mnode*> sets;
    int wallsDown = 0;
    for (uint i = 0; i < W ; ++i) {
        for (uint j = 0; j < H; ++j) {
            mnode* c = getNode(i, j);
            sets[c] = c;
            if (i < W - 1)
                edges.push_back(std::make_pair(c, getNode(i + 1, j)));
            if (j < H - 1)
                edges.push_back(std::make_pair(c, getNode(i, j+1)));
        }
    }
    //shuffle the edges:
    SHUFFLE(edges);
    // kruskal
    while (edges.size() && wallsDown < area-1) {
        mnode_edge cur = edges[edges.size()-1]; edges.pop_back();
        if (setFind(sets, cur.first) != setFind(sets, cur.second)) {
            setUnion(sets, cur.first, cur.second); // union the sets
            removeEdge(cur);
            ++wallsDown;
            ++steps;
            std::this_thread::sleep_for(std::chrono::milliseconds(delay));
        }
    }
    generated = true;
}

void Maze::addFrontier(mnode* c, std::deque<mnode*>& frontier, std::unordered_set<mnode*>& fset) {
    if (fset.find(c) != fset.end()) {
        printf("already in fset.\n");
        return;
    }
    frontier.push_back(c);
    fset.insert(c);
}

void Maze::addMst(mnode* c, int idx, std::deque<mnode*>& frontier, std::unordered_set<mnode*>& fset, std::unordered_set<mnode*>& mst) {
    if (mst.find(c) != mst.end()) return;
    if (fset.find(c) != fset.end())
        fset.erase(c);
    if (idx != -1) {
        frontier.erase(frontier.begin() + idx);
    }
    mst.insert(c);
    mnode_vec neighbours = unvisitedNeighbours(c);
    for (auto n : neighbours) {
        if (fset.find(n) == fset.end()) {
            addFrontier(n, frontier, fset);
        }
    }

}

void Maze::genPrims(uint& steps) {
    std::unordered_set<mnode*> mst; // minimal spanning tree
    std::deque<mnode*> frontier; // all mnodes adjacent to the mst
    std::unordered_set<mnode*> fset;
    mnode* a = data + rand() % area;
    addMst(a, -1, frontier, fset, mst);
    MNODE_VISIT(*a);
    while (frontier.size()) {
        // choose random frontier
        int idx = rand() % frontier.size();
        mnode* c = frontier[idx];
        addMst(c, idx, frontier, fset, mst);
        ++steps;
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
        // destroy the walls of one of the adjacent mst nodes (at random)
        MNODE_VISIT(*c);
        mnode_vec ins = visitedNeighbours(c);
        mnode* neighbour = ins[rand() % ins.size()];
        removeEdge(c, neighbour);
    }
    generated = true;
}

/* -------------------- SOLVERS -------------------- */
bool Maze::solveDFSHelper(mnode* c, uint& steps, uint& pathLen) {
    ++steps;
    std::this_thread::sleep_for(std::chrono::milliseconds(delay));
    ++pathLen;
    if (c == finish) return true;
    if (MNODE_PATH(*c)) return false;
    if (c != start)
        MNODE_SET_PATH(*c);
    mnode_vec accessible = accessibleNeighbours(c);
    for (auto n : accessible) 
        if (solveDFSHelper(n, steps, pathLen)) return true;
    if (c != start) {
        MNODE_SET_WASTED(*c);
        --pathLen;
    }
    return false;
}

void Maze::solveDFS(uint& steps, uint& pathLen) {
    solveDFSHelper(start, steps, pathLen);
    solved = true;
} 

void Maze::solveBFS(uint& steps, uint& pathLen) {
    std::queue<mnode*> q;
    std::unordered_map<mnode*, mnode*> path;
    mnode* current = start;
    while (current != finish) {
        ++steps;
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
        if (current != start)
            MNODE_SET_WASTED(*current);
        mnode_vec neighbours = accessibleNeighbours(current);
        for (auto neighbour : neighbours) {
            if (!MNODE_WASTED(*neighbour) && neighbour != start)  {
                q.push(neighbour);
                path[neighbour] = current;
            }
        }
        current = q.front(); q.pop();
    }
    while (current != start) {
        ++pathLen;
        current = path[current];
        if (current == start) break;
        MNODE_SET_PATH(*current);
    }
    solved = true;
}

double Maze::distCell(mnode* a, mnode* b) {
    coord diff = getCoord(a) - getCoord(b);
    return sqrt(diff.first*diff.first + diff.second*diff.second);
}

void Maze::solveAStar(uint& steps, uint& pathLen) {
    std::unordered_map<mnode*, double> cost;
    std::unordered_map<mnode*, mnode*> prev;
    std::priority_queue<std::pair<double, mnode*>, std::vector<std::pair<double, mnode*>>, std::greater<std::pair<double, mnode*>>> pq;
    cost[start] = 0 + distCell(start, finish);

    for (uint i = 0; i < area; ++i) {
        if (data + i!=start)cost[data + i] = DBL_MAX;
    }

    pq.push(std::make_pair(0, start));
    while (!pq.empty()) {
        mnode* u = pq.top().second;
        if (u == finish) {
            break;
        }
        if (u != start)
            MNODE_SET_WASTED(*u);
        ++steps;
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
        pq.pop();
        mnode_vec neighbours = accessibleNeighbours(u);
        for (auto v : neighbours) {
            if (cost[v] > cost[u] + distCell(u, finish)) {
                cost[v] = cost[u] + distCell(u, finish);
                prev[v] = u;
                pq.push(std::make_pair(cost[v], v));
            }
        }
    }
    // color path
    mnode* cur = prev[finish];
    while (cur != start) {
        ++pathLen;
        MNODE_SET_PATH(*cur);
        cur = prev[cur];
    }
    solved = true;
}

void Maze::solveDijkstra(uint& steps, uint& pathLen) {
    std::unordered_map<mnode*, unsigned int> distance;
    std::unordered_map<mnode*, mnode*> prev;
    std::priority_queue<std::pair<unsigned int, mnode*>, std::vector<std::pair<unsigned int, mnode*>>, std::greater<std::pair<unsigned int, mnode*>>> pq;
    distance[start] = 0;
    for (uint i = 0; i < area; ++i) {
        if (data + i != start) distance[data + i] = INT_MAX;
    }
    pq.push(std::make_pair(0, start));
    while (!pq.empty()) {
        mnode* u = pq.top().second;
        if (u == finish) {
            break;
        }
        if (u != start)
            MNODE_SET_WASTED(*u);
        pq.pop();
        mnode_vec neighbours = accessibleNeighbours(u);
        for (auto v : neighbours) {
            
            if (distance[v] > distance[u] + 1) {
                distance[v] = distance[u] + 1;
                prev[v] = u;
                pq.push(std::make_pair(distance[v], v));
            }
        }
    }
    // color path
    mnode* cur = prev[finish];
    while (cur != start) {
        MNODE_SET_PATH(*cur);
        cur = prev[cur];
    }
    solved = true;
}

void Maze::generate(const std::string& funcName, uint& steps) {
    void(Maze::*generator)(uint&) = GEN_DICT[funcName];
    (this->*generator)(steps);
}

void Maze::solve(const std::string& funcName, uint& steps, uint& pathLen) {
    void(Maze::*solver)(uint&,uint&) = SOLVE_DICT[funcName];
    (this->*solver)(steps, pathLen);
}

// std::thread Maze::genAsync(std::string func_name, uint& steps) {
//     void(Maze::*generator)(uint&) = GEN_DICT[func_name];
//     std::thread t([this, func_name, &steps, generator](){(this->*generator)(steps);});
//     return t;
// }
// std::thread Maze::solveAsync(std::string func_name, uint& steps, uint& pathLen) {
//     void(Maze::*solver)(uint&,uint&) = SOLVE_DICT[func_name];
//     std::thread t([this, func_name, &steps, &pathLen, solver](){(this->*solver)(steps, pathLen);});
//     return t;
// }