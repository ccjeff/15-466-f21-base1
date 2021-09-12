#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <memory>
#include "PPU466.hpp"
#include "Missle.hpp"
#include "assets/assets_res.h"

struct Enemy {
    Enemy(const glm::vec2 &start, const float init_speed, const int type) 
        : position(std::move(start)), speed(init_speed * 2), type(type) {}
    ~Enemy();
    bool update(float elapsed, std::vector<Missle> &missles);
    void draw(int offset, PPU466 &ppu);
    void fire(std::vector<Missle> &missles);
    glm::vec2 position;
    float speed;
    int type;
    int enemy_missle_num = 5;
    float shooting_cd = 1.0f;
};