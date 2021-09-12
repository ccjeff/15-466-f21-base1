#pragma once

#include "PPU466.hpp"
#include "Mode.hpp"
#include "Missle.hpp"
#include "Enemy.hpp"
#include "assets/assets_res.h"
#include "load_save_png.hpp"
#include "data_path.hpp"
#include "read_write_chunk.hpp"
#include<iostream>
#include <random>
#include <fstream>

#include <glm/glm.hpp>


#include <vector>
#include <deque>
#include <memory>

struct PlayMode : Mode {
	PlayMode();
	virtual ~PlayMode();

	//functions called by main loop:
	virtual bool handle_event(SDL_Event const &, glm::uvec2 const &window_size) override;
	virtual void update(float elapsed) override;
	virtual void draw(glm::uvec2 const &drawable_size) override;

	//----- game state -----

	//input tracking:
	struct Button {
		uint8_t downs = 0;
		uint8_t pressed = 0;
	} left, right, down, up;

	//some weird background animation:
	float background_fade = 0.0f;

	//player position: init
	glm::vec2 player_at = glm::vec2(0.0f);
	std::vector<Enemy> enemies;
	std::vector<std::shared_ptr<Missle>> missles; 
	
	uint8_t enemy_1_num = 5;
	uint8_t enemy_2_num = 5;
	
	

	//----- drawing handled by PPU466 -----

	PPU466 ppu;
};
