#include "Enemy.hpp"
#include "PlayMode.hpp"

Enemy::~Enemy(){}

void Enemy::update(float elapsed) {
    // update missle position
    this->position -= elapsed * this->speed * glm::normalize(this->position);
}

void Enemy::draw(int offset, PPU466& ppu) {
    if (this->type == 1) {
        ppu.sprites[offset].x = int32_t(this->position.x);
        ppu.sprites[offset].y = int32_t(this->position.y);
        ppu.sprites[offset].index = ENEMY1F_TILE_IDX;
        ppu.sprites[offset].attributes = ENEMY1F_PALETTE_IDX;
    }
}