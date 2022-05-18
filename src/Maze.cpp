#include "Maze.h"
#include "../deps/gif.h"

Maze::Maze(int w, int h, int _wallLen, int _cellLen) {
    W = w;
    H = h;
    
    wallLen = _wallLen;
    cellLen = _cellLen;
    iw = new ImageWriter(w*(wallLen + cellLen) + wallLen, h*(wallLen + cellLen) + wallLen);
    gw = new GifWriter;

    saveGif = true;
    gifDelay = DEFAULT_GIF_DELAY;

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
    if (saveGif) updateGif();
    Cell* nextCell = c->randomNeighbour();
    while (nextCell != nullptr) {
        c->destroyBorder(nextCell);
        dfsGenHelper(nextCell);
        nextCell = c->randomNeighbour();
    }
}

void Maze::genDFS() {
    if (saveGif) startGif("maze-gen(DFS).gif");
    dfsGenHelper(start);
    updateImage();
    saveImage("unsolved(DFS).png");
    if (saveGif) endGif();
}

void Maze::genKruskal() {
    // if (saveGif) startGif("maze-gen(kruskal).gif");
    // std::set<pair<Cell*, Cell*>> edges;
    // // load edges:
    // for (int i = 0; i < W; ++i) {
    //     for (int j = 0; j < H; ++j) {
    //         if (i + 1 < W)
    //             edges.insert(std::make_pair(&grid[i][j], &grid[]))
    //     }
    // }

    // updateImage();
    // saveImage("unsolved(kruskal).png");
    // if (saveGif) endGif();
}

/* -------------------- SOLVERS -------------------- */

bool Maze::solveDFSHelper(Cell* c) {
    if (c == finish) return true;
    if (c->getVal() == CELL_PATH) return false;
    if (c != start)
        c->setVal(CELL_PATH);
    if (saveGif) updateGif();
    std::vector<Cell*> accessible = c->accessibleNeighbours();
    for (auto n : accessible) 
        if (solveDFSHelper(n)) return true;
    c->setVal(CELL_WASTED);
    return false;
}

void Maze::solveDFS() {
    if (saveGif) startGif("solve(DFS).gif");
    solveDFSHelper(start);
    updateImage();
    saveImage("solved(DFS).png");
    if (saveGif) endGif();
} 

void Maze::solveBFS() {
    if (saveGif) startGif("solve(BFS).gif");
    std::queue<Cell*> q;
    std::unordered_map<Cell*, Cell*> path;
    Cell* current = start;
    while (current != finish) {
        if (saveGif) updateGif();
        if (current != start)
            current->setVal(CELL_WASTED);
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
        if (saveGif) updateGif();
    }
    updateImage();
    saveImage("solved(BFS).png");
    if (saveGif) endGif();
}
void Maze::solveAStar() {

}
void Maze::solveDijkstra() {

}

// image

void Maze::updateImage() {
	// draw the cells:
	for (int x = 0; x < getWidth(); ++x) {
		for (int y = 0; y < getHeight(); ++y) {
			std::pair<int, int> topLeft = std::make_pair(x*(cellLen + wallLen), y*(cellLen+wallLen));
			int outsideLen = cellLen+(wallLen*2);
			Cell* c = getCell(x, y);
            
			iw->fillRect(topLeft.first, topLeft.second, outsideLen, outsideLen, cell_colors[c->getVal()]);
			if (c->north()) iw->fillRect(topLeft.first, topLeft.second, outsideLen, wallLen, COLOR_BLACK);
			if (c->east()) iw->fillRect(topLeft.first + cellLen + wallLen, topLeft.second, wallLen, outsideLen, COLOR_BLACK);
			if (c->south()) iw->fillRect(topLeft.first, topLeft.second + cellLen + wallLen, outsideLen, wallLen, COLOR_BLACK);
			if (c->west()) iw->fillRect(topLeft.first, topLeft.second, wallLen, outsideLen, COLOR_BLACK);
			iw->fillRect(topLeft.first, topLeft.second, wallLen, wallLen, COLOR_BLACK);
		}
	}
}

void Maze::saveImage(const char* fname) {
    iw->save_to_file(fname);
}

void Maze::startGif(const char* gifName) {
    GifBegin(gw, gifName, iw->getWidth(), iw->getHeight(), gifDelay);
}

void Maze::updateGif() {
    updateImage();
    GifWriteFrame(gw, iw->getData(), iw->getWidth(), iw->getHeight(), gifDelay);
}

void Maze::endGif() {
    GifEnd(gw);
}