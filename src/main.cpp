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

void printUsage() {
    std::cout << "Mazer Usage:\n"
        <<"-g | generator algorithm *(DFS by default)*\n"
        <<"\tdfs\n\tkruskal\n"
        <<"-s | solving algorithm *(DFS by default)* \n"
        <<"\tdfs\n\tbfs\n"
        <<"-n | makes it so mazer doesnt save any gifs\n"
        <<"-w | width of cells the generated maze will have *(20 by default)*\n"
        <<"-h | height of cells the generated maze will have *(20 by default)*\n"
        <<"-r | choose the seed for random generation *(will change everytime by default)*\n"
        <<"-d | sets the delay of the gifs *in milliseconds*\n"
        <<"-o | specifies the directory mazer will output pictures and gifs to *(./ by default)*\n";
    exit(2);
}

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
    
    int opt;
    while ((opt = getopt(argc, argv, "g:s:nw:h:r:d:o:")) != -1) {
        switch (opt) {
            case 'g':
                generator = GEN_FUNCS[optarg];
                break;
            case 's':
                solver = SOLVE_FUNCS[optarg];
                break;
            case 'n':
                saveGif = false;
                break;
            case 'w':
                width = atoi(optarg);
                break;
            case 'h':
                height = atoi(optarg);
                break;
            case 'r':
                seed = atoi(optarg);
                break;
            case 'd':
                gifDelay = atoi(optarg);
                break;
            case 'o':
                dir = optarg;
                break;
            default:
                printUsage();
        }
    }
    
    srand(seed);
    Maze maze = Maze(width, height, cellLen, wallLen, saveGif, gifDelay);

    (maze.*generator)();
    (maze.*solver)();

    return 0;
}