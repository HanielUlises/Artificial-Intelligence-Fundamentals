#pragma once

#include "SDL.h"
#include "SDL_image.h"

#include <vector>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <thread>
#include <string>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 800;
const int GRID_SIZE = 16;
const int CELL_SIZE = SCREEN_WIDTH / GRID_SIZE;

class Agent {
public:
    Agent(SDL_Renderer* renderer, int size, int gridSize, int initialX, int initialY, const std::string& imagePath);
    bool isObstacle();
    bool isSample();

    void placeObstacle(int x, int y);

    void move();
    void stopMoving();
    void draw();

    int** initMatrix();

private:
    std::vector<std::pair<int, int>> obstacles;

    SDL_Renderer* renderer;
    SDL_Texture* texture;


    bool isMoving;
    // Counter of the samples recovered by the agents
    bool hasObject = false;

    int gridMatrix[GRID_SIZE][GRID_SIZE];
    // Positions
    int x, y;
    // Directions (movement)
    int dx, dy;
    // Tile size
    int size;
    // Grid size
    int gridSize;
   
};

void drawGrid(SDL_Renderer* renderer);
void handleEvents(SDL_Event& e, Agent& object1, Agent& object2, bool& quit);
void runProgram();
