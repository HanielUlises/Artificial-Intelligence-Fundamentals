#pragma once

#include "SDL.h"
#include "SDL_image.h"

#include <vector>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <thread>
#include <string> // Include string header for image path

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

    // Object color
    int redColor, blueColor, greenColor;

private:
    std::vector<std::pair<int, int>> obstacles;

    SDL_Renderer* renderer;
    SDL_Texture* texture;

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
    // Counter of the samples recovered by the agents
    int objectCounter = 0;

    
};

void drawGrid(SDL_Renderer* renderer);
void handleEvents(SDL_Event& e, Agent& object1, Agent& object2, bool& quit);
void runProgram();
