#include "Enemy.hpp"
#include "PlayMode.hpp"
#include "Mode.hpp"
#include <iostream>
#include <cmath>


Enemy::~Enemy(){}

bool Enemy::update(float elapsed) {
    // update enemy position
    static std::mt19937 mt; //mersenne twister pseudo-random number generator
    this->position.x -= mt()/float(mt.max()) * mt()/float(mt.max()) * speed* elapsed;
    this->position.x += mt()/float(mt.max()) * mt()/float(mt.max()) * speed* elapsed;
    this->position.y -= mt()/float(mt.max()) * speed * 2 * elapsed;
    this->position.y += mt()/float(mt.max()) * mt()/float(mt.max()) * speed* elapsed;
    // this->speed *= 0.995;
    this->position.x = std::min(this->position.x, 240.0f);
    // this->position.x = std::max(10.0f, this->position.x);
    this->position.y = std::min(this->position.y, 200.0f);
    // this->position.y = std::max(20.0f, this->position.x);
    if (this->position.x <= 5.0f || this->position.y <= 5.0f) {
        return true;
    }
    return false;
}

void Enemy::draw(int offset, PPU466& ppu) {
    if (this->type == 1) {
        ppu.sprites[offset].x = int32_t(this->position.x);
        ppu.sprites[offset].y = int32_t(this->position.y);
        ppu.sprites[offset].index = ENEMY1F_TILE_IDX;
        ppu.sprites[offset].attributes = ENEMY1F_PALETTE_IDX;
    } else if (this->type == 2){
        ppu.sprites[offset].x = int32_t(this->position.x);
        ppu.sprites[offset].y = int32_t(this->position.y);
        ppu.sprites[offset].index = ENEMY2F_TILE_IDX;
        ppu.sprites[offset].attributes = ENEMY2F_PALETTE_IDX;
    } else if (this->type == -1) {
        ppu.sprites[offset].x = int32_t(this->position.x);
        ppu.sprites[offset].y = int32_t(this->position.y);
        ppu.sprites[offset].index = BG_TILE_IDX;
        ppu.sprites[offset].attributes = BG_PALETTE_IDX;
    }
}