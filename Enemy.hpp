#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <memory>
#include "PPU466.hpp"
#include "assets/assets_res.h"

struct Enemy {
    Enemy(const glm::vec2 &start, const float init_speed, const int type) 
        : position(std::move(start)), speed(init_speed), type(type) {}
    ~Enemy();
    void update(float elapsed);
    void draw();
    glm::vec2 position;
    float speed;
    int type;
};