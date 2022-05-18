#pragma once

struct RGBA {
    unsigned char r, g, b, a;
};

const static RGBA white = RGBA{255, 255, 255, 255};
const static RGBA black = RGBA{0, 0, 0, 255};
const static RGBA red = RGBA{255, 0, 0, 255};
const static RGBA blue = RGBA{0, 0, 255, 255};
const static RGBA lightblue = RGBA{0, 120, 200, 255};
const static RGBA green = RGBA{0, 255, 0, 255};


#define DEFAULT_CHANNELS 4

const RGBA cell_colors[5] = {white, blue, lightblue, green, red};

class ImageWriter {
private: 
    int width, height, channels;
    unsigned char* data;

public: 
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