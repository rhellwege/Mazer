# Mazer
A minimalist commandline interface maze generator and solver. Note that all mazes generated are true traditional mazes where each cell is visited once.

## How to build
Make sure you have make installed, and simply run make in the console in this directory.
I'm pretty sure this is only buildable on linux.

## How to run
run mazer in the commandline with options
-w: width of cells the generated maze will have
-h: height of cells the generated maze will have
-g: generator algorithim
    DFS - depth first search generation.
-s: solving algorithim
    DFS - depth first search

## Output
Each generated image is color coded.
White represents an unvisited cell.
Black represents a wall
Dark blue represents the correct path
Light blue represents a tile that was visited during the solving phase, but is not included in the final path.
Green represents the starting cell, and red represents the ending cell.


## TODO
- [ ] getopt commandline options
- [ ] khruskhal generation
- [ ] animated gif