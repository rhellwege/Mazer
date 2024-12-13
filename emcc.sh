#!/bin/sh
em++ -std=c++17 src/main.cpp src/App.cpp src/Maze.cpp src/RayCaster.cpp src/vecop.cpp \
    deps/imgui/imgui.cpp \
    deps/imgui/imgui_draw.cpp \
    deps/imgui/imgui_widgets.cpp \
    deps/imgui/imgui_tables.cpp \
    deps/imgui/imgui_demo.cpp \
    deps/imgui/backends/imgui_impl_glfw.cpp \
    deps/imgui/backends/imgui_impl_opengl3.cpp \
    -I./include -I./deps -I./deps/imgui -I./deps/imgui/backends -I./deps/imgui/backends/GLFW -o public/index.html \
    -s USE_GLFW=3 \
    -s "EXPORTED_FUNCTIONS=['_main']" \
    -s "EXTRA_EXPORTED_RUNTIME_METHODS=['ccall', 'cwrap']" \
    -s "FULL_ES3=1" \
    -s "ALLOW_MEMORY_GROWTH=1" \
    -s "ENVIRONMENT='web'" \
    -s "WASM=1"
