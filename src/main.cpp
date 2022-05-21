#include "../include/common.h"
#include "../include/Maze.h"
#include <sys/stat.h>

#define DEFAULT_WIDTH 20
#define DEFAULT_HEIGHT 20
#define DEFAULT_CELL_LEN 8
#define DEFAULT_WALL_LEN 2
#define DEFAULT_SAVEGIF true
#define DEFAULT_GIF_DELAY 10
#define DEFAULT_GENERATOR "dfs"
#define DEFAULT_SOLVER "dfs"
#define DEFAULT_DIR "Mazer_output/"

std::unordered_map<std::string, void (Maze::*)(void)> GEN_FUNCS{
    {"dfs", &Maze::genDFS},
    {"kruskal", &Maze::genKruskal},
    {"kruskals", &Maze::genKruskal}
};

std::unordered_map<std::string, void (Maze::*)(void)> SOLVE_FUNCS{
    {"dfs", &Maze::solveDFS},
    {"bfs", &Maze::solveBFS},
    {"dijkstra", &Maze::solveDijkstra},
    {"dijkstras", &Maze::solveDijkstra},
    {"astar", &Maze::solveAStar},
    {"a*", &Maze::solveAStar},
    {"A*", &Maze::solveAStar}
};

void printUsage() {
    std::cout << "Mazer Usage:\n"
        <<"-g | generator algorithm *(DFS by default)*\n"
        <<"\tdfs\n\tkruskal\n"
        <<"-s | solving algorithm *(DFS by default)* \n"
        <<"\tdfs\n\tbfs\n\tdisjkstra\n\tastar\n"
        <<"-n | makes it so mazer doesnt save any gifs\n"
        <<"-w | width of cells the generated maze will have *(20 by default)*\n"
        <<"-h | height of cells the generated maze will have *(20 by default)*\n"
        <<"-r | choose the seed for random generation *(will change everytime by default)*\n"
        <<"-d | sets the delay of the gifs *in milliseconds*\n"
        <<"-o | specifies the directory mazer will output pictures and gifs to *(Mazer_output/ by default)*\n"
        <<"-c | sets the width of each cell in pixels *(8 pixels by default)*\n"
        <<"-l | sets the width of the walls *(2 pixels by default)*\n";
    exit(2);
}

int main(int argc, char** argv) {
    int width = DEFAULT_WIDTH;
    int height = DEFAULT_HEIGHT;
    int cellLen = DEFAULT_CELL_LEN;
    int wallLen = DEFAULT_WALL_LEN;
    bool saveGif = DEFAULT_SAVEGIF;
    int gifDelay = DEFAULT_GIF_DELAY;

    void(Maze::*generator)(void) = GEN_FUNCS[DEFAULT_GENERATOR];
    void(Maze::*solver)(void) = SOLVE_FUNCS[DEFAULT_SOLVER];
    std::string s_gen = DEFAULT_GENERATOR;
    std::string s_solve = DEFAULT_SOLVER;

    unsigned int seed = time(NULL);
    std::string dir = DEFAULT_DIR;
    if (dir[dir.size()-1] != '/') dir+="/";
    
    int opt;
    while ((opt = getopt(argc, argv, "g:s:nw:h:r:d:o:c:l:")) != -1) {
        switch (opt) {
            case 'g':
                if (GEN_FUNCS.find(optarg) == GEN_FUNCS.end()) printUsage();
                generator = GEN_FUNCS[optarg];
                s_gen = optarg;
                break;
            case 's':
                if (SOLVE_FUNCS.find(optarg) == SOLVE_FUNCS.end()) printUsage();
                solver = SOLVE_FUNCS[optarg];
                s_solve = optarg;
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
                mode_t m;
                mkdir(dir.c_str(), m);
                break;
            case 'c':
                cellLen = atoi(optarg);
                break;
            case 'l':
                wallLen = atoi(optarg);
                break;
            default:
                printUsage();
        }
    }
    
    srand(seed);
    Maze maze = Maze(width, height, cellLen, wallLen, saveGif, gifDelay, seed, dir);
    auto t0 = std::chrono::steady_clock::now();
    (maze.*generator)();
    auto t1 = std::chrono::steady_clock::now();
    maze.saveImage((dir + s_gen  + "-gen.png").c_str());
    auto t2 = std::chrono::steady_clock::now(); 
    (maze.*solver)();
    auto t3 = std::chrono::steady_clock::now();
    maze.saveImage((dir + s_solve  + "-solve.png").c_str());

    double elapsed_gen = double(std::chrono::duration_cast<std::chrono::nanoseconds> (t1 - t0).count()) / 1e9;
    double elapsed_solve = double(std::chrono::duration_cast<std::chrono::nanoseconds> (t2 - t1).count()) / 1e9;

    std::cout << s_gen << " Generation took " << elapsed_gen << " seconds to complete.\n";
    std::cout << s_solve << " Solve took " << elapsed_solve << " seconds to complete.\n";

    FILE* f_ptr = fopen((dir + "info.txt").c_str(), "w");
    fprintf(f_ptr, "Generator: %s\nSolver: %s\n\nWidth: %i\nHeight: %i\n\nSeed: %i\n\nGeneration time: %f\nSolve time: %f", s_gen.c_str(), s_solve.c_str(), width, height, seed, elapsed_gen, elapsed_solve);
    fclose(f_ptr);
    return 0;
}