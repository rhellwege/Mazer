#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "../include/Maze.h"

#define DEFAULT_WIDTH 20
#define DEFAULT_HEIGHT 20
#define DEFAULT_CELL_LEN 8
#define DEFAULT_WALL_LEN 2
#define DEFAULT_SAVEGIF true
#define DEFAULT_GIF_DELAY 10
#define DEFAULT_GENERATOR "dfs"
#define DEFAULT_SOLVER "dfs"

int main(int argc, char** argv) {
    std::unordered_map<std::string, void (Maze::*)(void)> GEN_FUNCS;
    GEN_FUNCS["dfs"] = &Maze::genDFS;
    GEN_FUNCS["kruskal"] = &Maze::genKruskal;
    GEN_FUNCS["kruskals"] = &Maze::genKruskal;

    std::unordered_map<std::string, void (Maze::*)(void)> SOLVE_FUNCS;
    SOLVE_FUNCS["dfs"] = &Maze::solveDFS;
    SOLVE_FUNCS["bfs"] = &Maze::solveBFS;

    int width = DEFAULT_WIDTH;
    int height = DEFAULT_HEIGHT;
    int cellLen = DEFAULT_CELL_LEN;
    int wallLen = DEFAULT_WALL_LEN;
    bool saveGif = DEFAULT_SAVEGIF;
    int gifDelay = DEFAULT_GIF_DELAY;

    void(Maze::*generator)(void) = GEN_FUNCS[DEFAULT_GENERATOR];
    void(Maze::*solver)(void) = SOLVE_FUNCS[DEFAULT_SOLVER];

    unsigned int seed = time(NULL);
    std::string dir;
    

    // int flags, opt;
    // int nsecs, tfnd;

    // while ((opt) = getopt(argc, argv, "tttt") != -1)
    
    srand(seed);
    Maze maze = Maze(width, height, cellLen, wallLen, saveGif, gifDelay);

    (maze.*generator)();
    (maze.*solver)();

    return 0;
}