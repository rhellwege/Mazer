#pragma once

struct RGBA {
    unsigned char r, g, b, a;
};

const static RGBA COLOR_WHITE = RGBA{255, 255, 255, 255};
const static RGBA COLOR_BLACK = RGBA{0, 0, 0, 255};
const static RGBA COLOR_RED = RGBA{255, 0, 0, 255};
const static RGBA COLOR_BLUE = RGBA{0, 0, 255, 255};
const static RGBA COLOR_LIGHTBLUE = RGBA{0, 120, 200, 255};
const static RGBA COLOR_GREEN = RGBA{0, 255, 0, 255};


#define DEFAULT_CHANNELS 4

const RGBA cell_colors[5] = {COLOR_WHITE, COLOR_BLUE, COLOR_LIGHTBLUE, COLOR_GREEN, COLOR_RED};

class ImageWriter {
private: 
    int width, height, channels;
    unsigned char* data;

public: 
    ImageWriter();
    ImageWriter(int w, int h, int _channels = DEFAULT_CHANNELS);
    ~ImageWriter();
    void setPixel(int x, int y, RGBA col);
    void fillRect(int x, int y, int w, int h, RGBA col);
    int getWidth();
    int getHeight();
    int getChannels();
    void resize(int w, int h);
    void freeImage();
    unsigned char* getData();
    void save_to_file(const char* fname);
};