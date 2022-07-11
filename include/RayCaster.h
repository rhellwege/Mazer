#pragma once
#include "common.h"
#include "Maze.h"
#include "imgui.h"
// all angles are in *RADIANS*
struct Ray {
    bool isHorizontalHit;
    float dist;
};

struct Wall {
    ImU32 color;
    float height;
};

class RayCaster {
private:
    Maze* maze;
    float castWidth;
    float walkspeed;
    float rotspeed;

public:
    ImVec2 pos, dir, plane;
    ImVec2 canvas_p0, canvas_sz; // screen dimensions
    RayCaster(Maze* m);
    void reset();
    Wall getWall(Ray r);
    void walk(int d);
    void rotate(bool right);
    Ray cast(float planeRatio);
    void display();
};