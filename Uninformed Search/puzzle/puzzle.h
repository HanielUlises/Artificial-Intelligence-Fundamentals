#pragma once
#include "iostream"
#include "string.h"

#include "SDL.h"
#include "SDL_image.h"

#undef main

const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 1000;

// Here is the definition of the puzzle to be solved
// It can be solved by BFS of DFS
class Puzzle {
public:

};

// The window to be displayed with the sub-images
class Board {
public:
	Board(SDL_Renderer * renderer, const std::string &imagePath);
	~Board();

	void Draw(SDL_Renderer* renderer);
private: 
	SDL_Renderer* renderer;
	SDL_Texture* texture;
};

void handleEvents();
void runProgram();