#pragma once
#include "common.h"
#include "Maze.h"
// all angles are in *RADIANS*
struct Ray {
    bool isHorizontalHit;
    float dist;
};

struct Player {
    float x, y, angle;
};

class RayCaster {
private:
    Player player;
    Maze* maze;

public:
    RayCaster(Maze* m);
    void walk(float units, float delta = 0.0f);
    void turn(float delta);
    Ray cast(float delta);
};