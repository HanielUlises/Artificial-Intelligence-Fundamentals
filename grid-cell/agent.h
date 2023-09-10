#pragma once

#include <SDL.h>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <thread>

class Agent {
public:
    bool moving;

    Agent(SDL_Renderer* renderer, int size, int gridSize, int initialX, int initialY);


    void moveRandomly();

    void move();

    void draw();

private:
    SDL_Renderer* renderer;
    int x, y;
    int dx, dy;
    int size;
    int gridSize;
};

void drawGrid();
void windowDisplay();