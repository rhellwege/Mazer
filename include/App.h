#pragma once
#include "../deps/imgui/imgui.h"
#include "Maze.h"
#include "../deps/imgui/backends/GLFW/glfw3.h"

static const ImU32 bg_col = IM_COL32(255, 255, 255, 255);
static const ImU32 wall_col = IM_COL32(0, 0, 0, 255);
static const ImU32 start_col = IM_COL32(0, 255, 0, 255);
static const ImU32 finish_col = IM_COL32(255, 0, 0, 255);
static const ImU32 wasted_col = IM_COL32(180, 180, 255, 255);
static const ImU32 path_col = IM_COL32(0, 0, 255, 255);
static const ImU32 active_col = IM_COL32(255, 255, 0, 255);

struct Ray {
    bool isHorizontalHit;
    float originX, originY;
    float hitX, hitY;
    float trueDist;
    float camDist;
};

struct Player {
    float x, y, angle;
};

class App {
private:
    const char* window_title;
    int window_width;
    int window_height;
    GLFWwindow* window;
    Maze* maze;
    bool show_info;
    bool show_maze;
    bool show_controls;
    bool show_demo;
    bool show_menubar;
    bool show_raycast;
    bool maze_generating;
    bool maze_solving;
    uint steps_gen;
    uint steps_solve;
    uint len_path;
    float cell_to_wall;
    float timescale;
    ImU32 getFillCol(mnode* m);
    inline void resetMaze();
    Player player;

    Ray castFromPlayer(float delta);
    ImU32 getCastCol(float dist);

public:
    App(const char* title, int width, int height);
    ~App();
    void renderControls();
    void renderInfo();
    void renderMaze();
    void renderRayCast();
    void renderMenuBar();
    void run();

    void savePNG();
};