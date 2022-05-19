#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "../include/Maze.h"

int main(int argc, char** argv) {
    unsigned int seed = time(NULL);
    const char* dir;
    

    // int flags, opt;
    // int nsecs, tfnd;

    // while ((opt) = getopt(argc, argv, "tttt") != -1)
    
    srand(seed);
    Maze maze = Maze(10, 10);
    maze.genDFS();
    maze.solveDFS();
    return 0;
}