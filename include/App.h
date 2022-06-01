#pragma once
#include "../deps/imgui/imgui.h"
#include "Maze.h"
#include "../deps/imgui/backends/GLFW/glfw3.h"

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
    bool maze_generating;
    bool maze_solving;
    bool animate_generation;
    bool animate_solving;
    float playback_speed;
    uint steps_gen;
    uint steps_solve;
    uint len_path;
    float cell_to_wall;
public:
    App(const char* title, int width, int height);
    ~App();
    void renderControls();
    void renderInfo();
    void renderMaze();
    void run();
};