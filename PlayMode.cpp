#include "PlayMode.hpp"

//for the GL_ERRORS() macro:
#include "gl_errors.hpp"
//for glm::value_ptr() :
#include <glm/gtc/type_ptr.hpp>
#include <cmath>
#include <chrono>
#include <thread>
#include <array>

PlayMode::PlayMode() {
	// you *must* use an asset pipeline of some sort to generate tiles.
	// don't hardcode them like this!
	// or, at least, if you do hardcode them like this,
	//  make yourself a script that spits out the code that you paste in here
	//   and check that script into your repository.
	// reference: https://github.com/xuxiaoqiao/15-466-f20-base1/blob/HEAD/PlayMode.cpp
	// used with modification for reading chunk files
	std::vector<PPU466::Tile> tile_input;
	std::vector<PPU466::Palette> palette_input;

	std::ifstream tile_stream(data_path("assets/tiles.chunk"), std::ios::binary);
	std::ifstream palette_stream(data_path("assets/palettes.chunk"), std::ios::binary);

	if (tile_stream.is_open() && palette_stream.is_open()){
		std::cout<<"Input stream open success"<<std::endl;
	}
	read_chunk(tile_stream, "til0", &tile_input);
	read_chunk(palette_stream, "plt0", &palette_input);
	std::copy(tile_input.begin(),tile_input.end(), ppu.tile_table.begin());
	std::copy(palette_input.begin(), palette_input.end(), ppu.palette_table.begin());

	for (uint32_t i = 0; i < ppu.background.size(); ++i) {
		ppu.background[i] = int16_t(
			BG_PALETTE_IDX << 8
			| BG_TILE_IDX
		);
	}
	static std::mt19937 mt; //mersenne twister pseudo-random number generator
	for (int i = 0; i < enemy_1_num; ++i) {
		glm::vec2 pos(mt()/float(mt.max()) * PPU466::ScreenWidth, std::max(mt()/float(mt.max()) * PPU466::ScreenHeight, 0.5f*PPU466::ScreenHeight));
		this->enemies.emplace_back(pos, 5, 1);
	}
	for (int i = 0; i < enemy_2_num; ++i) {
		glm::vec2 pos(mt()/float(mt.max()) * PPU466::ScreenWidth, std::max(mt()/float(mt.max()) * PPU466::ScreenHeight, 0.5f*PPU466::ScreenHeight));
		this->enemies.emplace_back(pos, 5, 2);
	}

}

PlayMode::~PlayMode() {
}

bool PlayMode::handle_event(SDL_Event const &evt, glm::uvec2 const &window_size) {

	if (evt.type == SDL_KEYDOWN) {
		if (evt.key.keysym.sym == SDLK_LEFT) {
			left.downs += 1;
			left.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_RIGHT) {
			right.downs += 1;
			right.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_UP) {
			up.downs += 1;
			up.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_DOWN) {
			down.downs += 1;
			down.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_SPACE) {
			space.downs += 1;
			space.pressed = true;
			return true;
		}
	} else if (evt.type == SDL_KEYUP) {
		if (evt.key.keysym.sym == SDLK_LEFT) {
			left.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_RIGHT) {
			right.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_UP) {
			up.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_DOWN) {
			down.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_SPACE) {
			space.pressed = false;
			return true;
		}
	}
	return false;
}

void PlayMode::update(float elapsed) {

	//slowly rotates through [0,1):
	// (will be used to set background color)
	// background_fade += elapsed / 10.0f;
	// background_fade -= std::floor(background_fade);

	constexpr float PlayerSpeed = 30.0f;
	if (left.pressed) player_at.x -= PlayerSpeed * elapsed;
	if (right.pressed) player_at.x += PlayerSpeed * elapsed;
	if (down.pressed) player_at.y -= PlayerSpeed * elapsed;
	if (up.pressed) player_at.y += PlayerSpeed * elapsed;

	cd -= elapsed;
	if (space.pressed) {
		if (cd <= 0) {
				glm::vec2 pos(player_at.x, player_at.y + 8);
				this->missles.emplace_back(pos, 50.0f, 1);
				cd = 0.5f;
			}
	}

	for (auto e = this->enemies.begin(); e != this->enemies.end();) {
		this->is_over = e->update(elapsed, this->missles);
		if (this->is_over) break;
		if (e->type == -1) {
			e = this->enemies.erase(e);
		} else {
			++e;
		}
	}

	for (auto m = this->missles.begin(); m != this->missles.end();) {
		if (this->is_over) break;
		if (m->position.x <= player_at.x + 4 && m->position.x >= player_at.x - 4 &&
				m->position.y <= player_at.y + 4 && m->position.y >= player_at.y - 4 && m->creator == 2) {
			// player dies
			--player_health;
			if (player_health < 1) {
				is_over = true;
				break;
			}
		}
		for (auto &e : this->enemies) {
			if (m->position.x <= ((int)e.position.x % (int)PPU466::ScreenWidth) + 4 && (int)m->position.x >= ((int)e.position.x % (int)PPU466::ScreenWidth) - 4 &&
				m->position.y <= ((int)e.position.y % (int)PPU466::ScreenHeight) + 4 && (int)m->position.y >= ((int)e.position.y % (int)PPU466::ScreenHeight) - 4 && m->creator == 1) {
				// kill enemy
				if (e.type == 1) curr_enemy1--;
				if (e.type == 2) curr_enemy2--;
				e.type = -1;
				++score;
			}
		}
		if (m->creator == -1) {
			m = this->missles.erase(m);
		} else {
			m->update(elapsed);
			++m;
		}
	}

	static std::mt19937 mt; //mersenne twister pseudo-random number generator
	for (;curr_enemy1 < enemy_1_num; ++curr_enemy1) {
		if (this->is_over) break;
		glm::vec2 pos(mt()/float(mt.max()) * (PPU466::ScreenWidth + ppu.background_position.x), std::max(mt()/float(mt.max()) * (PPU466::ScreenHeight + ppu.background_position.y), 0.7f*(PPU466::ScreenHeight + ppu.background_position.y)));
		this->enemies.emplace_back(pos, 5, 1);
	}
	for (;curr_enemy2 < enemy_2_num; ++curr_enemy2) {
		if (this->is_over) break;
		glm::vec2 pos(mt()/float(mt.max()) * (PPU466::ScreenWidth + ppu.background_position.x), std::max(mt()/float(mt.max()) * (PPU466::ScreenHeight + ppu.background_position.y), 0.7f*(PPU466::ScreenHeight + ppu.background_position.y)));
		this->enemies.emplace_back(pos, 5, 2);
	}
	// assert(this->enemies.size() == enemy_1_num + enemy_2_num);

	//reset button press counters:
	left.downs = 0;
	right.downs = 0;
	up.downs = 0;
	down.downs = 0;
	space.downs = 0;

	if (this->is_over) {
		std::this_thread::sleep_for(std::chrono::milliseconds(5000));
		Mode::current = nullptr;
		return;
	}
}

void PlayMode::draw(glm::uvec2 const &drawable_size) {
	//--- set ppu state based on game state ---
	ppu.background_color = glm::u8vec4(0xff, 0xff, 0xff, 0xff);

	//background scroll:
	ppu.background_position.x = int32_t(-0.5f * player_at.x);
	ppu.background_position.y = int32_t(-0.5f * player_at.y);

	//player sprite:
	ppu.sprites[0].x = int32_t(player_at.x);
	ppu.sprites[0].y = int32_t(player_at.y);
	ppu.sprites[0].index = PLANE_LL_TILE_IDX;
	ppu.sprites[0].attributes = PLANE_LL_PALETTE_IDX;

	ppu.sprites[1].x = int32_t(player_at.x + 8);
	ppu.sprites[1].y = int32_t(player_at.y);
	ppu.sprites[1].index = PLANE_LR_TILE_IDX;
	ppu.sprites[1].attributes = PLANE_LL_PALETTE_IDX;

	ppu.sprites[2].x = int32_t(player_at.x);
	ppu.sprites[2].y = int32_t(player_at.y + 8);
	ppu.sprites[2].index = PLANE_UL_TILE_IDX;
	ppu.sprites[2].attributes = PLANE_UL_PALETTE_IDX;

	ppu.sprites[3].x = int32_t(player_at.x + 8);
	ppu.sprites[3].y = int32_t(player_at.y + 8);
	ppu.sprites[3].index = PLANE_UR_TILE_IDX;
	ppu.sprites[3].attributes = PLANE_UR_PALETTE_IDX;

	int counter = 4;
	for (auto &m : this->missles) {
		m.draw(counter++, ppu);
	}
	for (auto &e : this->enemies) {
		e.draw(counter++, ppu);
	}

	//reference:  https://github.com/xuxiaoqiao/15-466-f20-base1/blob/HEAD/PlayMode.cpp
	// drawing score tiles on the top
	
	constexpr int SCORE_DISPLAY_WIDTH = 3;
	static const int MAX_HEALTH = 5;
	constexpr std::array<uint8_t, 10> NUMBERS_TILE_IDX = {
		ZERO_TILE_IDX,
		ONE_TILE_IDX,
		TWO_TILE_IDX,
		THREE_TILE_IDX,
		FOUR_TILE_IDX,
		FIVE_TILE_IDX,
		SIX_TILE_IDX,
		SEVEN_TILE_IDX,
		EIGHT_TILE_IDX,
		NINE_TILE_IDX
	};
	constexpr std::array<uint8_t, 10> NUMBERS_PALETTE_IDX = {
		ZERO_PALETTE_IDX,
		ONE_PALETTE_IDX,
		TWO_PALETTE_IDX,
		THREE_PALETTE_IDX,
		FOUR_PALETTE_IDX,
		FIVE_PALETTE_IDX,
		SIX_PALETTE_IDX,
		SEVEN_PALETTE_IDX,
		EIGHT_PALETTE_IDX,
		NINE_PALETTE_IDX
	};

	std::array<int, 3> score_separate_digits;
		if (score >= 1000) {
			score_separate_digits = {9, 9, 9};
		} else {
			score_separate_digits = {(score / 100) % 10, (score / 10) % 10, score % 10};
		}

		for (int i = 0; i < SCORE_DISPLAY_WIDTH; i++) {
			ppu.sprites[counter + i].x = 255 - 8 * 3 + i * 8;
			ppu.sprites[counter + i].y = 239 - 8;
			ppu.sprites[counter + i].index = NUMBERS_TILE_IDX.at(score_separate_digits.at(i));
			ppu.sprites[counter + i].attributes = NUMBERS_PALETTE_IDX.at(score_separate_digits.at(i));
			++counter;
		}

		for (int i = 0; i < MAX_HEALTH; i++) {
			if (i <= this->player_health) {
				ppu.sprites[counter + i].x = i * 8;
				ppu.sprites[counter + i].y = 239 - 8;
				ppu.sprites[counter + i].index = PLANE_SMALL_TILE_IDX;
				ppu.sprites[counter + i].attributes = PLANE_SMALL_PALETTE_IDX;
			} else {
				ppu.sprites[counter + i].x = i * 8;
				ppu.sprites[counter + i].y = 239 - 8;
				ppu.sprites[counter + i].index = BG_TILE_IDX;
				ppu.sprites[counter + i].attributes = BG_PALETTE_IDX;
			}
			++counter;
		}
	
	//--- actually draw ---
	ppu.draw(drawable_size);
}
