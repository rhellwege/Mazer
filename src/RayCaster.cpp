#include "RayCaster.h"
#include "settings.h"
#include "vecop.h"

#define PI 3.14159265358979
#define TWO_PI PI*2
#define HALF_PI PI/2
#define THREE_PI_OVER_TWO 3*PI/2
#define RD 0.01745329

RayCaster::RayCaster(Maze* m) {
    pos.x = m->full_sz.x/2, pos.y = m->full_sz.y/2;
    dir.x = -1.0f, dir.y = 0.0f;
    plane.x = 0.0f, plane.y = 0.66;
    castWidth = DEFAULT_CAST_WIDTH;
    maze = m;
    walkspeed = 1;
    rotspeed = 0.1;
}

void RayCaster::walk(int d) {
    ImVec2 newPos = pos + (dir * walkspeed);
    if (maze->isWall(newPos)) return;
    pos = newPos;
}

void RayCaster::rotate(bool right) {
    if (right) {
        float oldDirX = dir.x;
        dir.x = dir.x * cos(-rotspeed) - dir.y * sin(-rotspeed);
        dir.y = oldDirX * sin(-rotspeed) + dir.y * cos(-rotspeed);
        float oldPlaneX = plane.x;
        plane.x = plane.x * cos(-rotspeed) - plane.y * sin(-rotspeed);
        plane.y = oldPlaneX * sin(-rotspeed) + plane.y * cos(-rotspeed);
    }
    else {
        double oldDirX = dir.x;
        dir.x = dir.x * cos(rotspeed) - dir.y * sin(rotspeed);
        dir.y = oldDirX * sin(rotspeed) + dir.y * cos(rotspeed);
        double oldPlaneX = plane.x;
        plane.x = plane.x * cos(rotspeed) - plane.y * sin(rotspeed);
        plane.y = oldPlaneX * sin(rotspeed) + plane.y * cos(rotspeed);
    }
}

Ray RayCaster::cast(float planeRatio) {
    Ray hit;
    int n = 0;
    int side;
    ImVec2 raydir;
    float perpWallDist;
    int stepx, stepy;
    ImVec2 sideDist, deltaDist;
    ImVec2 map;
    float camx = 2 * planeRatio - 1;
    raydir.x = dir.x + plane.x * camx;
    raydir.y = dir.y + plane.y * camx;
    map.x = (int)(pos.x / maze->full_sz.x);
    map.y = (int)(pos.y / maze->full_sz.y);
    deltaDist.x = (raydir.x == 0) ? 1e30 : std::abs(1 / raydir.x);
    deltaDist.y = (raydir.y == 0) ? 1e30 : std::abs(1 / raydir.y);
    // calc step and initial side distance
    if (raydir.x < 0) {
        stepx = -1;
        sideDist.x = (pos.x - map.x) * deltaDist.x;
    }
    else {
        stepx = 1;
        sideDist.x = (map.x + 1.0 - pos.x) * deltaDist.x;
    }
    if (raydir.y < 0) {
        stepy = -1;
        sideDist.y = (pos.y - map.y) * deltaDist.y;
    }
    else {
        stepy = 1;
        sideDist.y = (map.y + 1.0 - pos.y) * deltaDist.y;
    }
    while (n == 0) {
        if (sideDist.x < sideDist.y) {
            sideDist.x += deltaDist.x;
            map.x += stepx;
            side = 0;
        }
        else {
            sideDist.y += deltaDist.y;
            map.y += stepy;
            side = 1;
        }
        if (maze->isWall(map)) n = 1;
    }
    if (side == 0) perpWallDist = (sideDist.x - deltaDist.x);
    else           perpWallDist = (sideDist.y - deltaDist.y);
    if (side == 0) hit.isHorizontalHit = true;
    else hit.isHorizontalHit = false;
    hit.dist = perpWallDist;
    //f//loat alpha = delta + angle;
    return hit;
}

Wall RayCaster::getWall(Ray r) {
    Wall w;
    if (r.isHorizontalHit) w.color = horz_col;
    else w.color = vert_col;
    w.height = canvas_sz.y / r.dist;
    return w;
}

void RayCaster::reset() {
    pos.x = maze->full_sz.x/2, pos.y = maze->full_sz.y/2;
    dir.x = -1.0f, dir.y = 0.0f;
    plane.x = 0.0f, plane.y = 0.66;
}

void RayCaster::display() {
    ImGui::Begin("Ray Cast");
    ImGui::SliderFloat("Cast Width", &castWidth, 1.0f, 30.0f);
    if (ImGui::Button("Rotate Left")) {
        rotate(false);
    }
    ImGui::SameLine();
    if (ImGui::Button("Rotate Right")) {
        rotate(true);
    }
    ImGui::SameLine();
    if (ImGui::Button("Walk")) {
        walk(0);
    }
    ImGui::SameLine();
    if (ImGui::Button("reset")) {
        reset();
    }
    //float* fpos = (float*)&pos;
    //ImGui::SliderFloat2("position", fpos, 0.0f, maze->getWidth());
    canvas_p0 = ImGui::GetCursorScreenPos();      // ImDrawList API uses screen coordinates!
    canvas_sz = ImGui::GetContentRegionAvail();   // Resize canvas to what's available
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
    // draw background colors (sky and floor)
    draw_list->AddRectFilled(canvas_p0, ImVec2(canvas_p1.x, canvas_p1.y-canvas_sz.y/2), sky_col);
    draw_list->AddRectFilled(ImVec2(canvas_p0.x, canvas_p0.y + canvas_sz.y/2), canvas_p1, floor_col);
    float total = canvas_sz.x/castWidth;
    
    for (uint i = 0; i < total; ++i) {
        Ray hit = cast(i/total);
        Wall wall = getWall(hit);
        ImVec2 wall_p0, wall_p1;
        wall_p0.x = canvas_p0.x + castWidth * i;
        wall_p0.y = canvas_p0.y + (canvas_sz.y / 2 - wall.height / 2);
        wall_p1.x = wall_p0.x + castWidth;
        wall_p1.y = wall_p0.y + wall.height;
        wall_p1.x = fmin(wall_p1.x, canvas_p1.x);
        wall_p1.y = fmin(wall_p1.y, canvas_p1.y);
        wall_p0.y = fmax(canvas_p0.y, wall_p0.y);
        draw_list->AddRectFilled(wall_p0, wall_p1, wall.color);
    }
    ImGui::End();
}