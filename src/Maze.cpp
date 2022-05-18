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
    if (saveGif) startGif("maze-gen.gif");
    dfsGenHelper(start);
    start->destroyWall(0);
    finish->destroyWall(2);
    updateImage();
    saveImage("unsolved.png");
    if (saveGif) endGif();
}

void Maze::genUnion() {

}
void Maze::genKhruskhal() {

}

/* -------------------- SOLVERS -------------------- */

bool Maze::solveDFSHelper(Cell* c) {
    if (c == finish) return true;
    if (c->getVal() == 1) return false;
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
    if (saveGif) startGif("solve.gif");
    solveDFSHelper(start);
    updateImage();
    saveImage("solved.png");
    if (saveGif) endGif();
} 

void Maze::solveBFS() {

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