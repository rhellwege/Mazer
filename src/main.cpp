#include "../include/Maze.h"
#include "../include/Image.h"

int main() {
    srand(time(NULL));
    Maze maze = Maze(20, 20);
    Image i(maze);
    maze.genDFS();
    i.save_to_file("unsolved.png");
    maze.solveDFS();
    i.update(maze);
    i.save_to_file("solved.png");
    
    return 0;
}