#pragma once

#include <SDL.h>

#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <thread>

class Agent {
public:
    Agent(SDL_Renderer* renderer, int size, int gridSize, int initialX, int initialY);
    void moveRandomly();
    void move();
    void draw();
    void initMatrix();
    
    // Uhmmm
    bool moving;
private:
    SDL_Renderer* renderer;

    int gridMatrix[16][16];
    int x, y;
    int dx, dy;
    int size;
    int gridSize;
  
};

void drawGrid();
void windowDisplay();