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
#include <queue>
#include <set>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 800;
const int GRID_SIZE = 16;
const int CELL_SIZE = SCREEN_WIDTH / GRID_SIZE;

// Node structure for A* algorithm
struct Node {
    int x, y; // Grid coordinates
    int g; // Cost from start to this node
    int h; // Heuristic (estimated cost to goal)
    int f; // Total cost (g + h)
    Node* parent; // Parent node for path reconstruction

    Node(int x, int y, int g = 0, int h = 0, Node* parent = nullptr)
        : x(x), y(y), g(g), h(h), f(g + h), parent(parent) {}

    // For priority queue comparison (min-heap based on f-cost)
    bool operator<(const Node& other) const {
        return f > other.f; // Min-heap: lower f-cost has higher priority
    }
};

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
    bool hasSample = false;
    int x, y;
    int dx, dy;
    int size;
    int gridSize;

    // A* related members
    std::vector<std::pair<int, int>> path; // Current path to follow
    bool findPathAStar(int startX, int startY, int goalX, int goalY, int gridMatrix[][GRID_SIZE]); // A* pathfinding
    int heuristic(int x1, int y1, int x2, int y2); // Manhattan distance heuristic
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
void drawSamples(SDL_Renderer* renderer, const std::vector<std::pair<int, int>>& samples, int size);
void handleEvents(SDL_Event& e, std::vector<std::pair<int, int>>& obstacles, std::vector<std::pair<int, int>>& samples, Agent& object1, Agent& object2, bool& quit);
void handleMouseClick(SDL_Event& e, std::vector<std::pair<int, int>>& obstacles, std::vector<std::pair<int, int>>& samples);
void runProgram();