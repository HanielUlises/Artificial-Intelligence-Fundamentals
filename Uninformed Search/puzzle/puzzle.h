#pragma once
#include "iostream"
#include "vector"
#include "cstdlib"
#include "string.h"

#include "SDL.h"
#include "SDL_image.h"

#undef main

const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 1000;

const int PUZZLE_ROWS = 4;
const int PUZZLE_COLS = 4;

// Stack data structure that stores the paths
struct Stack {
	std::vector<std::string> paths;

	std::string top() const {
		if (!paths.empty()) {
			return paths.back();
		}
		else {
			throw std::runtime_error("Stack is empty\n");
		}
	}

	bool isEmpty() const {
		return paths.empty();
	}

	void push(const std::string& path) {
		paths.push_back(path);
	}

	void pop() {
		if (!(paths.empty())) {
			paths.pop_back();
		}
		else {
			throw std::runtime_error("Stack is empty\n");
		}
	}

	size_t size() const {
		return paths.size();
	}
};

// Here is the definition of the puzzle to be solved
// It can be solved by BFS of DFS
class Puzzle {
public:

};

// The window to be displayed with the sub-images
class Board {
public:
	enum class Direction {
		UP,
		DOWN,
		LEFT,
		RIGHT
	};

	Board(SDL_Renderer * renderer, const std::string &imagePath);
	~Board();

	void Draw(SDL_Renderer* renderer);
	void ShufflePuzzle();
	void FindEmptyTile(int& row, int& col);
	void MoveEmptySpace(Direction direction);
private: 
	

	int emptyRow;
	int emptyCol;

	SDL_Renderer* renderer;
	SDL_Texture* texture;
};

void handleEvents();
void runProgram();