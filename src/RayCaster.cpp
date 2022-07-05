#include "RayCaster.h"
#include "settings.h"

#define PI 3.14159265358979
#define TWO_PI PI*2
#define HALF_PI PI/2
#define THREE_PI_OVER_TWO 3*PI/2
#define RD 0.01745329

RayCaster::RayCaster(Maze* m) {
    x = 0.5f, y =0.5f, angle = 0.0f;
    castWidth = DEFAULT_CAST_WIDTH;
    maze = m;
}

void RayCaster::walk(float units, float delta) {
    y += sin(angle+delta)*units;
    x += cos(angle+delta)*units;
}

void RayCaster::turn(float delta) {
    angle += delta;
}

Ray RayCaster::cast(float delta) {
    Ray r;
    return r;
}

void RayCaster::display() {
    ImGui::Begin("Ray Cast");

    static float fov = 90.0f;
    static float angle = 0.0f;

    ImGui::SliderFloat("FOV", &fov, 60.0f, 180.0f);
    ImGui::SliderFloat("Angle", &angle, 0.0f, 360.0f);
    ImGui::SliderFloat("Cast Width", &castWidth, 1.0f, 30.0f);

    ImVec2 canvas_p0 = ImGui::GetCursorScreenPos();      // ImDrawList API uses screen coordinates!
    ImVec2 canvas_sz = ImGui::GetContentRegionAvail();   // Resize canvas to what's available
    if (canvas_sz.x < 50.0f) canvas_sz.x = 50.0f;
    if (canvas_sz.y < 50.0f) canvas_sz.y = 50.0f;
    ImVec2 canvas_p1 = ImVec2(canvas_p0.x + canvas_sz.x, canvas_p0.y + canvas_sz.y);
    // Draw border and background color
    ImGuiIO& io = ImGui::GetIO();
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    // This will catch our interactions
    ImGui::InvisibleButton("canvas", canvas_sz, ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
    const bool is_hovered = ImGui::IsItemHovered(); // Hovered
    const bool is_active = ImGui::IsItemActive();   // Held
    ImVec2 mouse_pos_in_canvas(io.MousePos.x - canvas_p0.x, io.MousePos.y - canvas_p0.y);
    // draw grid
    draw_list->AddRectFilled(canvas_p0, canvas_p1, wall_col);
    
    float rfov = RD*fov;
    float delta = rfov / (canvas_sz.x / castWidth);
    for (uint i = 0; i < canvas_sz.x/castWidth; ++i) {
        Ray result = cast(delta);
        
    }
    ImGui::End();
}