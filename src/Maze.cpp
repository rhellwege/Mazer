#include "../include/Maze.h"

Maze::Maze(int w, int h, int _wallLen, int _cellLen) {
    W = w;
    H = h;
    iw = ImageWriter(w, h);
    wallLen = _wallLen;
    cellLen = _cellLen;
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

void dfsGenHelper(Cell* c) {
    if (c->seen()) return;
    c->visit();
    Cell* nextCell = c->randomNeighbour();
    while (nextCell != nullptr) {
        c->destroyBorder(nextCell);
        dfsGenHelper(nextCell);
        nextCell = c->randomNeighbour();
    }
}

void Maze::genDFS() {
    dfsGenHelper(start);
    start->destroyWall(0);
    finish->destroyWall(2);
    updateImage();
    saveImage("unsolved.png");
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
    std::vector<Cell*> accessible = c->accessibleNeighbours();
    for (auto n : accessible) 
        if (solveDFSHelper(n)) return true;
    c->setVal(CELL_WASTED);
    return false;
}

void Maze::solveDFS() {
   solveDFSHelper(start);
   updateImage();
   saveImage("solved.png");
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
			iw.fillRect(topLeft.first, topLeft.second, outsideLen, outsideLen, cell_colors[c->getVal()]);
			if (c->north()) iw.fillRect(topLeft.first, topLeft.second, outsideLen, wallLen, black);
			if (c->east()) iw.fillRect(topLeft.first + cellLen + wallLen, topLeft.second, wallLen, outsideLen, black);
			if (c->south()) iw.fillRect(topLeft.first, topLeft.second + cellLen + wallLen, outsideLen, wallLen, black);
			if (c->west()) iw.fillRect(topLeft.first, topLeft.second, wallLen, outsideLen, black);
			iw.fillRect(topLeft.first, topLeft.second, wallLen, wallLen, black);
		}
	}
}

void Maze::saveImage(const char* fname) {
    iw.save_to_file(fname);
}