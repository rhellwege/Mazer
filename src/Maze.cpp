#include "Maze.h"
#include "../deps/gif.h"

Maze::Maze(int w , int h, int _cellLen, int _wallLen, bool _saveGif, int _gifDelay, unsigned int _seed, std::string _dir) {
    W = w;
    H = h;
    
    wallLen = _wallLen;
    cellLen = _cellLen;
    seed = _seed;

    gifDelay = _gifDelay;
    saveGif = _saveGif;

    dir = _dir;

    iw = new ImageWriter(w*(wallLen + cellLen) + wallLen, h*(wallLen + cellLen) + wallLen);
    gw = new GifWriter;

    for (int i = 0; i < w; ++i) {
        grid.push_back({});
        for (int j = 0; j < h; ++j) {
            grid[i].push_back({i, j, w, h, 0, &grid});
        }
    }

    start = &grid[W/2][0];
    finish = &grid[W/2][H-1];
    start->setVal(CELL_START);
    finish->setVal(CELL_FINISH);
    imgSetup();
    updateImage();
}

int Maze::getVal(int x, int y) {
    return grid[x][y].getVal();
}

int Maze::getHeight() {
    return H;
}

int Maze::getWidth() {
    return W;
}

Cell* Maze::getCell(int x, int y) {
    return &grid[x][y];
}

/* -------------------- GENERATORS -------------------- */
void Maze::dfsGenHelper(Cell* c) {
    if (c->seen()) return;
    c->visit();
    
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
    int steps = 0;
    if (saveGif) startGif((dir + "solve(DFS).gif").c_str());
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

double Maze::distCell(Cell* a, Cell* b) {
    std::pair<double, double> diff = std::make_pair(b->getX() - a->getX(), b->getY() - a->getY());
    return sqrt(diff.first*diff.first + diff.second*diff.second);
}

int Maze::solveAStar() {
    int steps = 0;
    if (saveGif) startGif((dir + "solve(Astar).gif").c_str());
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

int Maze::solveDijkstra() {
    if (saveGif) startGif((dir + "solve(Dijkstra).gif").c_str());
    int steps = 0;
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

// image

void Maze::updateBorderPixels(Cell* c, int direction, RGBA col) {
    int x = c->getX(); 
    int y = c->getY();
    std::pair<int, int> topLeft = std::make_pair(x*(cellLen + wallLen), y*(cellLen+wallLen));
    switch  (direction) {
        case 0:
            iw->fillRect(topLeft.first + wallLen, topLeft.second, cellLen, wallLen, col);
            break;
        case 1:
            iw->fillRect(topLeft.first + cellLen + wallLen, topLeft.second + wallLen, wallLen, cellLen, col);
            break;
        case 2:
            iw->fillRect(topLeft.first + wallLen, topLeft.second + cellLen + wallLen, cellLen, wallLen, col);
            break;
        case 3:
            iw->fillRect(topLeft.first, topLeft.second + wallLen, wallLen, cellLen, col);
    }
}

void Maze::imgSetup()  { // draws all corners once so we dont repeat it
    for (int x = 0; x < getWidth(); ++x) {
		for (int y = 0; y < getHeight(); ++y) {
        std::pair<int, int> tl = std::make_pair(x*(cellLen + wallLen), y*(cellLen+wallLen));
		iw->fillRect(tl.first, tl.second, wallLen, wallLen, COLOR_BLACK); // corner
        if (x == W-1) iw->fillRect(tl.first + wallLen + cellLen, tl.second, wallLen, wallLen, COLOR_BLACK);
        if (y == H-1) iw->fillRect(tl.first, tl.second + wallLen + cellLen, wallLen, wallLen, COLOR_BLACK);
        if (x == W-1 && y == H-1) iw->fillRect(tl.first + wallLen + cellLen, tl.second + wallLen + cellLen, wallLen, wallLen, COLOR_BLACK);
		}
	}
}

void Maze::updateCellCol(Cell* c) { // only updates pixels that arent border
    if  (c->getVal() == CELL_UNVISITED) return; // we memset every pixel to white in imagewriter so we dont need to draw white anywhere.
    RGBA col = cell_colors[c->getVal()];

    int x = c->getX();
    int y = c->getY();

    std::pair<int, int> topLeft = std::make_pair(x*(cellLen + wallLen), y*(cellLen+wallLen));
    RGBA* otherCol = iw->getPixel(topLeft.first + wallLen, topLeft.second + wallLen);
    if (col.r == otherCol->r && col.g == otherCol->g && col.b == otherCol->b) return;
    std::pair<int, int> bottomRight = std::make_pair(topLeft.first + cellLen + wallLen, topLeft.second + cellLen + wallLen);

    std::pair<int, int> tl = topLeft;

    iw->fillRect(topLeft.first + wallLen, topLeft.second + wallLen, cellLen, cellLen, col);
    if (!c->getWall(NORTH)) updateBorderPixels(c, 0, col);
    if (!c->getWall(EAST)) updateBorderPixels(c, 1, col);
    if (!c->getWall(SOUTH)) updateBorderPixels(c, 2, col);
    if (!c->getWall(WEST)) updateBorderPixels(c, 3, col);
}

void Maze::updateCellPixels(Cell* c) {
    updateCellCol(c);

	if (c->getWall(NORTH)) updateBorderPixels(c, 0, COLOR_BLACK);
    else updateBorderPixels(c, 0, COLOR_WHITE);
	if (c->getWall(EAST)) updateBorderPixels(c, 1, COLOR_BLACK);
    else updateBorderPixels(c, 1, COLOR_WHITE);
	if (c->getWall(SOUTH)) updateBorderPixels(c, 2, COLOR_BLACK);
    else updateBorderPixels(c, 2, COLOR_WHITE);
	if (c->getWall(WEST)) updateBorderPixels(c, 3, COLOR_BLACK);
    else updateBorderPixels(c, 3, COLOR_WHITE);
}

void Maze::updateImage() {
	// draw the cells:
	for (int x = 0; x < getWidth(); ++x) {
		for (int y = 0; y < getHeight(); ++y) {
			updateCellPixels(getCell(x,y));
		}
	}
}

void Maze::saveImage(const char* fname) {
    iw->save_to_file(fname);
}

void Maze::startGif(const char* gifName) {
    printf("Starting GIF %s...\n", gifName);
    GifBegin(gw, gifName, iw->getWidth(), iw->getHeight(), gifDelay);
}

void Maze::addFrame() {
    GifWriteFrame(gw, iw->getData(), iw->getWidth(), iw->getHeight(), gifDelay);
}

void Maze::updateGif() {
    updateImage();
    addFrame();
}

void Maze::endGif() {
    GifEnd(gw);
}