#pragma once

#include "SDL.h"

#include "iostream"
#include "cstdlib"
#include "ctime"
#include "chrono"
#include "thread"

class Agent {
public:
    Agent(SDL_Renderer* renderer, int size, int gridSize, int initialX, int initialY);
    bool isObstacle();
    bool isSample();

    void moveRandomly();
    void move();
    void stopMoving();
    void draw();
    void initMatrix();
    
private:
    SDL_Renderer* renderer;

    bool isMoving;

    int gridMatrix[16][16];
    int x, y;
    int dx, dy;
    int size;
    int gridSize;
  
};

void drawGrid();
void windowDisplay();