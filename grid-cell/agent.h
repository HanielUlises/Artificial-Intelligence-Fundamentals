#pragma once

#include "SDL.h"

#include "vector"
#include "iostream"
#include "cstdlib"
#include "ctime"
#include "chrono"
#include "thread"

class Agent {
public:
    Agent(SDL_Renderer* renderer, int size, int gridSize, int initialX, int initialY, int redColor, int blueColor, int greenColor);
    bool isObstacle();
    bool isSample();

    void placeObstacle(int x, int y);

    void move();
    void stopMoving();
    void draw();

    int** initMatrix();

    // Object color
    int redColor, blueColor, greenColor;

private:
    std::vector<std::pair<int, int>> obstacles;

    SDL_Renderer* renderer;

    bool isMoving;

    int gridMatrix[16][16];
    // Positions
    int x, y;
    // Directions (movement)
    int dx, dy;
    // Tile size
    int size;
    // Grid size
    int gridSize;

};

void drawGrid(SDL_Renderer* renderer, int redColor, int blueColor, int greenColor);
void handleEvents();
void runProgram();