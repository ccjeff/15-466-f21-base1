#include "PlayMode.hpp"

//for the GL_ERRORS() macro:
#include "gl_errors.hpp"
//for glm::value_ptr() :
#include <glm/gtc/type_ptr.hpp>

// reference: https://github.com/xuxiaoqiao/15-466-f20-base1/blob/HEAD/PlayMode.cpp
// used with modification for reading chunk files
PlayMode::PlayMode() {
	// you *must* use an asset pipeline of some sort to generate tiles.
	// don't hardcode them like this!
	// or, at least, if you do hardcode them like this,
	//  make yourself a script that spits out the code that you paste in here
	//   and check that script into your repository.
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
		glm::vec2 pos(mt()/float(mt.max()) * PPU466::ScreenWidth, mt()/float(mt.max()) * PPU466::ScreenHeight);
		this->enemies.emplace_back(pos, 5, 1);
	}
	for (int i = 0; i < enemy_2_num; ++i) {
		glm::vec2 pos(mt()/float(mt.max()) * PPU466::ScreenWidth, mt()/float(mt.max()) * PPU466::ScreenHeight);
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
		}
	}
	return false;
}

void PlayMode::update(float elapsed) {

	//slowly rotates through [0,1):
	// (will be used to set background color)
	background_fade += elapsed / 10.0f;
	background_fade -= std::floor(background_fade);

	constexpr float PlayerSpeed = 30.0f;
	if (left.pressed) player_at.x -= PlayerSpeed * elapsed;
	if (right.pressed) player_at.x += PlayerSpeed * elapsed;
	if (down.pressed) player_at.y -= PlayerSpeed * elapsed;
	if (up.pressed) player_at.y += PlayerSpeed * elapsed;

	//reset button press counters:
	left.downs = 0;
	right.downs = 0;
	up.downs = 0;
	down.downs = 0;
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
	ppu.sprites[0].index = PLANE_SMALL_TILE_IDX;
	ppu.sprites[0].attributes = PLANE_SMALL_PALETTE_IDX;

	int counter = 1;
	for (auto e :this->enemies) {
		e.draw(counter++, ppu);
	}

	//--- actually draw ---
	ppu.draw(drawable_size);
}
