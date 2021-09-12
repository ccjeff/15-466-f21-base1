#include "Missle.hpp"

Missle::~Missle(){};

void Missle::update(float elapsed) {
    // update missle position
    this->position -= elapsed * this->speed * glm::normalize(this->position);
}