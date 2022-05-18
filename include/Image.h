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

const RGB cell_colors[5] = {white, blue, lightblue, green, red};

class Image {
private: 
    int width, height, channels, stride, wallLen, cellLen;
    unsigned char* data;
    void setPixel(int x, int y, RGB col);
    void fillRect(int x, int y, int w, int h, RGB col);

public: 
    Image(Maze& m, int _cellLen, int _wallLen, int _channels);
    ~Image();
    int getWidth();
    int getHeight();
    int getChannels();
    int getWallLen();
    int getCellLen();
    unsigned char* getData();
    void save_to_file(const char* fname);
};