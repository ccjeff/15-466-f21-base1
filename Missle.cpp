#include "Missle.hpp"

Missle::~Missle(){}

void Missle::update(float elapsed) {
    // update missle position
    if (this->creator == 1) {
        this->position.y += elapsed * this->speed * 5;
        if (this->position.y >= 240) {
            this->creator = -1;
        }
    } else if (this->creator == 2) {
        this->position.y -= elapsed * this->speed * 2;
        if (this->position.y <= 0) {
            this->creator = -1;
        }
    }
    
}

void Missle::draw(int offset, PPU466 &ppu) {
    if (this->creator == 1) {
        ppu.sprites[offset].x = int32_t(this->position.x);
        ppu.sprites[offset].y = int32_t(this->position.y);
        ppu.sprites[offset].index = MISSLE_TILE_IDX;
        ppu.sprites[offset].attributes = MISSLE_PALETTE_IDX;
    } else if (this->creator == 2){
        ppu.sprites[offset].x = int32_t(this->position.x);
        ppu.sprites[offset].y = int32_t(this->position.y);
        ppu.sprites[offset].index = MISSLEF_TILE_IDX;
        ppu.sprites[offset].attributes = MISSLEF_PALETTE_IDX;
    }
}