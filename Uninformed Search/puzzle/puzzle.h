#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <SDL.h>
#include <SDL_image.h>

const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 1000;
const int PUZZLE_ROWS = 4;
const int PUZZLE_COLS = 4;

class Puzzle {
public:
    std::vector<Board::Direction> solve(const int initialState[PUZZLE_ROWS][PUZZLE_COLS]);
};

class Board {
public:
    enum class Direction { UP, DOWN, LEFT, RIGHT };
    Board(SDL_Renderer* renderer, const std::string& imagePath);
    ~Board();
    void Draw(SDL_Renderer* renderer);
    void ShufflePuzzle();
    void FindEmptyTile(int& row, int& col);
    void MoveEmptySpace(Direction direction);

    std::vector<Direction> solutionPath;
    Uint32 lastMoveTime;

private:
    SDL_Renderer* renderer;
    SDL_Texture* texture;
};

void handleEvents(SDL_Event& e, bool& quit, Board& board);
void runProgram();