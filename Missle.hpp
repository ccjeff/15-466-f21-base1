#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <memory>
#include "PPU466.hpp"
#include "assets/assets_res.h"

struct Missle {
    Missle(const glm::vec2 &start, const float init_speed, const int creator) 
        : position(std::move(start)), speed(init_speed), creator(creator) {}
    ~Missle();
    void update(float elapsed);
    void draw(int offset, PPU466 &ppu);
    glm::vec2 position;
    float speed;
    int creator;
};