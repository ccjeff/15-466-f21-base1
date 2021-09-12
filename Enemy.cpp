#include "Enemy.hpp"

Enemy::~Enemy(){}

void Enemy::update(float elapsed) {
    // update missle position
    this->position -= elapsed * this->speed * glm::normalize(this->position);
}

void Enemy::draw() {
    
}