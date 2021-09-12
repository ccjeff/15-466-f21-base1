#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <memory>
#include "PPU466.hpp"

struct Missle {
    Missle(const glm::vec2 &start, const glm::vec2 &init_speed) {}
    ~Missle();
    void update(float elapsed);
    void draw();
    glm::vec2 position;
    float speed;
};