#pragma once
#include "common.h"
#include "Maze.h"
#include "imgui.h"
// all angles are in *RADIANS*
struct Ray {
    bool isHorizontalHit;
    float dist;
};

class RayCaster {
private:
    Maze* maze;
    float x, y, angle;
    float castWidth;
    float fov;

public:
    ImU32 getCastCol(float dist);
    RayCaster(Maze* m);
    void walk(float units, float delta = 0.0f);
    void turn(float delta);
    Ray cast(float delta);
    void display();
};