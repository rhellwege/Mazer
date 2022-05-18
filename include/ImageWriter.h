#pragma once
#include "Maze.h"

struct RGB {
    unsigned char r, g, b;
};

const RGB white = RGB{255, 255, 255};
const RGB black = RGB{0, 0, 0};
const RGB red = RGB{255, 0, 0};
const RGB blue = RGB{0, 0, 255};
const RGB lightblue = RGB{0, 120, 200};
const RGB green = RGB{0, 255, 0};

#define DEFAULT_CELL_LEN 8
#define DEFAULT_WALL_LEN 2
#define DEFAULT_CHANNELS 3

const RGB cell_colors[5] = {white, blue, lightblue, green, red};

class ImageWriter {
private: 
    int width, height, channels, wallLen, cellLen;
    unsigned char* data;

public: 
    ImageWriter(int _cellLen = DEFAULT_CELL_LEN, int _wallLen = DEFAULT_WALL_LEN, int _channels = DEFAULT_CHANNELS);
    ImageWriter(Maze& m, int _cellLen = DEFAULT_CELL_LEN, int _wallLen = DEFAULT_WALL_LEN, int _channels = DEFAULT_CHANNELS);
    ~ImageWriter();
    void setPixel(int x, int y, RGB col);
    void fillRect(int x, int y, int w, int h, RGB col);
    void update(Maze& m);
    int getWidth();
    int getHeight();
    int getChannels();
    int getWallLen();
    int getCellLen();
    unsigned char* getData();
    void save_to_file(const char* fname);
};