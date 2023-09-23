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
    ~Agent();
    bool isObstacle();
    bool isSample(int gridMatrix[][GRID_SIZE]);
    bool gotSample();
  
    void move(int gridMatrix[][GRID_SIZE]);
    void moveTowardsShip(int shipX, int shipY, int gridMatrix[][GRID_SIZE]);
    void stopMoving();

    void draw();

    void pickSample(std::vector<std::pair<int, int>>& activeSamples, std::vector<std::pair<int, int>>& samples, int gridMatrix[][GRID_SIZE]);
    void dropSample();

    int** initMatrix();

    int getX();
    int getY();

    std::vector<std::pair<int, int>>& getActiveSamples();


private:
    std::vector<std::pair<int, int>> obstacles;
    std::vector<std::pair<int, int>> activeSamples;

    SDL_Renderer* renderer;
    SDL_Texture* texture;

    int** localMatrix = new int* [3];
    bool isMoving;
    // Counter of the samples recovered by the agents
    bool hasSample = false;
    // Positions
    int x, y;
    // Directions (movement)
    int dx, dy;
    // Tile size
    int size;
    // Grid size
    int gridSize;
   
};

class Ship {
public:
    Ship(SDL_Renderer* renderer, int size, int initialX, int initialY, const std::string& imagePath);
    ~Ship();

    void draw();
private:
    SDL_Renderer* renderer;
    SDL_Texture* texture;

    int x, y;
    int size;
};

void drawObstacles(SDL_Renderer* renderer, const std::vector<std::pair<int, int>>& obstacles, int size, int gridMatrix[][GRID_SIZE]);
void initializeSamples(std::vector<std::pair<int, int>>& samples, int numSamples);
void drawSamples(SDL_Renderer* renderer, std::vector<std::pair<int, int>>& samples, int size, int x, int y, int gridMatrix[][GRID_SIZE]);

void handleEvents(SDL_Event& e, std::vector<std::pair<int, int>>& obstacles, Agent& object1, Agent& object2, bool& quit);
void handleMouseClick(SDL_Event& e, std::vector<std::pair<int, int>>& obstacles, const std::vector<std::pair<int, int>>& samples);
void runProgram();
