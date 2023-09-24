#include "puzzle.h"

Board::Board(SDL_Renderer* renderer, const std::string& imagePath) {
	this->renderer = renderer;
	SDL_Surface* imageSurface = IMG_Load(imagePath.c_str());
	if (imageSurface == nullptr) {
		std::cerr << "Failed to load image: " << IMG_GetError() << std::endl;
	}
	else {
		texture = SDL_CreateTextureFromSurface(renderer, imageSurface);
		SDL_FreeSurface(imageSurface);
	}
}

Board::~Board() {
	if (texture != nullptr) {
		SDL_DestroyTexture(texture);
		texture = nullptr;
	}
}

void Board::Draw(SDL_Renderer* renderer) {
	SDL_RenderCopy(renderer, texture, nullptr, nullptr);
}

void handleEvents(SDL_Event& e, bool& quit) {
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			quit = true;
		}
	}
}

void runProgram() {
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window* window = SDL_CreateWindow("Puzzle BFS/DFS", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	SDL_Event e;

	bool quit = false;

	Board newBoard(renderer, "image/gatito.png");
	while (!quit) {
		handleEvents(e, quit);

		SDL_RenderClear(renderer);

		newBoard.Draw(renderer);

		SDL_RenderPresent(renderer);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}