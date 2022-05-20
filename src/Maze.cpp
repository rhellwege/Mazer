#include "Maze.h"
#include "../deps/gif.h"

Maze::Maze(int w , int h, int _cellLen, int _wallLen, bool _saveGif, int _gifDelay, unsigned int _seed) {
    W = w;
    H = h;
    
    wallLen = _wallLen;
    cellLen = _cellLen;
    seed = _seed;

    gifDelay = _gifDelay;
    saveGif = _saveGif;

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
    start->destroyWall(0);
    finish->destroyWall(2);
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
    if (saveGif) startGif("solve.gif");
    dfsGenHelper(start);
    if (!saveGif)
        updateImage();
    saveImage("unsolved(DFS).png");
    //if (saveGif) endGif();
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
    if (saveGif) startGif("solve.gif");
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
    saveImage("unsolved(kruskal).png");
}

/* -------------------- SOLVERS -------------------- */

bool Maze::solveDFSHelper(Cell* c) {
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
        if (solveDFSHelper(n)) return true;
    if (c != start)
        c->setVal(CELL_WASTED);
    if (saveGif) {
        updateCellCol(c);
        addFrame();
    }
    return false;
}

void Maze::solveDFS() {
    //if (saveGif) startGif("solve(DFS).gif");
    solveDFSHelper(start);
    //updateImage();
    if (!saveGif) updateImage();
    saveImage("solved(DFS).png");
    if (saveGif) endGif();
} 

void Maze::solveBFS() {
    //if (saveGif) startGif("solve(BFS).gif");
    std::queue<Cell*> q;
    std::unordered_map<Cell*, Cell*> path;
    Cell* current = start;
    while (current != finish) {
        
        if (current != start)
            current->setVal(CELL_WASTED);
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
    saveImage("solved(BFS).png");
    if (saveGif) endGif();
}
void Maze::solveAStar() {

}
void Maze::solveDijkstra() {

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
    if (!c->north()) updateBorderPixels(c, 0, col);
    if (!c->east()) updateBorderPixels(c, 1, col);
    if (!c->south()) updateBorderPixels(c, 2, col);
    if (!c->west()) updateBorderPixels(c, 3, col);
}

void Maze::updateCellPixels(Cell* c) {
    updateCellCol(c);

	if (c->north()) updateBorderPixels(c, 0, COLOR_BLACK);
	if (c->east()) updateBorderPixels(c, 1, COLOR_BLACK);
	if (c->south()) updateBorderPixels(c, 2, COLOR_BLACK);
	if (c->west()) updateBorderPixels(c, 3, COLOR_BLACK);
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