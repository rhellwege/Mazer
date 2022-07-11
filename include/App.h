#pragma once
#include "../deps/imgui/imgui.h"
#include "Maze.h"
#include "../deps/imgui/backends/GLFW/glfw3.h"
#include "RayCaster.h"

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
    
    float timescale;
    
    inline void resetMaze();
    RayCaster* caster;
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