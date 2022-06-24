#include "RayCaster.h"

RayCaster::RayCaster(Maze* m) {
    player = Player{0.5f, 0.5f, 0.0f};
    maze = m;
}

void RayCaster::walk(float units, float delta) {
    player.y += sin(player.angle+delta)*units;
    player.x += cos(player.angle+delta)*units;
}

void RayCaster::turn(float delta) {
    player.angle += delta;
}