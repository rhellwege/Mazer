#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "../include/Maze.h"
#include "../include/ImageWriter.h"

int main(int argc, char** argv) {
    // int flags, opt;
    // int nsecs, tfnd;

    // while ((opt) = getopt(argc, argv, "tttt") != -1)
    
    srand(time(NULL));
    Maze maze = Maze(20, 20);
    ImageWriter i;
    maze.genDFS();
    i.update(maze);
    i.save_to_file("unsolved.png");
    maze.solveDFS();
    i.update(maze);
    i.save_to_file("solved.png");
    
    return 0;
}