#pragma once
#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 1000
#define WINDOW_TITLE "Mazer GUI"
#define MAZE_WIDTH 10
#define MAZE_HEIGHT 10
#define DEFAULT_DELAY 300
#define DEFAULT_ZOOM 2.0f
#define DEFAULT_CAST_WIDTH 8
#define DEFAULT_FOV 90.0f
static const ImU32 bg_col = IM_COL32(255, 255, 255, 255);
static const ImU32 wall_col = IM_COL32(0, 0, 0, 255);
static const ImU32 start_col = IM_COL32(0, 255, 0, 255);
static const ImU32 finish_col = IM_COL32(255, 0, 0, 255);
static const ImU32 wasted_col = IM_COL32(180, 180, 255, 255);
static const ImU32 path_col = IM_COL32(0, 0, 255, 255);
static const ImU32 active_col = IM_COL32(255, 255, 0, 255);