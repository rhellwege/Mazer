#pragma once
#include "../deps/gif.h"

#define DEFAULT_DELAY 100

class GifSaver {
private:
    int delay;
    const char* fname;
public:
    GifSaver(const char* _fname, int _delay = DEFAULT_DELAY);
};