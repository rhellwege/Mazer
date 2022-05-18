#include "../include/Maze.h"
#include "../include/Image.h"

int main() {
    srand(time(NULL));
    Maze maze = Maze(20, 20);
    maze.genDFS();
    maze.solveDFS();
    Image i = Image(maze, 8, 1, 3);
    i.save_to_file("test.png");
    return 0;
}