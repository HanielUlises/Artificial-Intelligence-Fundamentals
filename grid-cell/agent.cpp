#include "agent.h"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 800;
const int GRID_SIZE = 16;
const int CELL_SIZE = SCREEN_WIDTH / GRID_SIZE;

Agent::Agent(SDL_Renderer *renderer, int size, int gridSize, int initialX, int initialY) :
    renderer(renderer),
    size(size),
    gridSize(gridSize),
    x(initialX),
    y(initialY) {
    dx = (rand() % 3 - 1) * CELL_SIZE; // Random initial velocity
    dy = (rand() % 3 - 1) * CELL_SIZE;
    moving = true; // Object starts moving initially
}

void Agent::moveRandomly() {
    dx = (rand() % 3 - 1) * CELL_SIZE; // Adjust the velocity randomly
    dy = (rand() % 3 - 1) * CELL_SIZE;
}

void Agent::move() {
    if (!moving) {
        return; // If not moving, do nothing
    }

    int nextX = x + dx;
    int nextY = y + dy;

    // Check for collision with the window boundaries
    if (nextX >= 0 && nextX < SCREEN_WIDTH && nextY >= 0 && nextY < SCREEN_HEIGHT) {
        x = nextX;
        y = nextY;
    }
    else {
        // Object is out of bounds, bounce off the walls

        // Check for horizontal wall collision
        if (nextX < 0 || nextX >= SCREEN_WIDTH) {
            dx = -dx; // Reverse the horizontal velocity to bounce
        }

        // Check for vertical wall collision
        if (nextY < 0 || nextY >= SCREEN_HEIGHT) {
            dy = -dy; // Reverse the vertical velocity to bounce
        }

        // Update the position after bouncing
        x = x + dx;
        y = y + dy;
    }
}
void Agent::stopMoving(){
    !isMoving;
}

void Agent::draw() {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red color
    SDL_Rect rect = { x, y, size, size };
    SDL_RenderFillRect(renderer, &rect);
}

void Agent::initMatrix() {
    int localMatrix[3][3] = {
        {gridMatrix[x - 1][y - 1], gridMatrix[x][y - 1], gridMatrix[x + 1][y - 1]},
        {gridMatrix[x - 1][y], gridMatrix[x][y], gridMatrix[x + 1][y]},
        {gridMatrix[x - 1][y + 1], gridMatrix[x][y + 1], gridMatrix[x + 1][y + 1]}
    };
}


void drawGrid(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Set color to black

    // Vertical lines
    for (int i = 1; i < GRID_SIZE; ++i) {
        int x = i * CELL_SIZE;
        SDL_RenderDrawLine(renderer, x, 0, x, SCREEN_HEIGHT);
    }

    // Horizontal lines
    for (int i = 1; i < GRID_SIZE; ++i) {
        int y = i * CELL_SIZE;
        SDL_RenderDrawLine(renderer, 0, y, SCREEN_WIDTH, y);
    }
}

void windowDisplay() {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Moving Objects", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    bool quit = false;
    SDL_Event e;

    srand(static_cast<unsigned>(time(nullptr)));

    // Specify initial positions for object1 and object2
    int initialX1 = 100;
    int initialY1 = 100;
    int initialX2 = 200;
    int initialY2 = 200;

    Agent object1(renderer, CELL_SIZE, GRID_SIZE, initialX1, initialY1);
    Agent object2(renderer, CELL_SIZE, GRID_SIZE, initialX2, initialY2);

    auto lastMoveTime = std::chrono::high_resolution_clock::now();
    const double moveInterval = 0.2; // Move every [amount] of secondsss

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
            else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_SPACE) {
                    object1.stopMoving();
                    object2.stopMoving();
                }
            }
        }

        auto currentTime = std::chrono::high_resolution_clock::now();
        double elapsedSeconds = std::chrono::duration<double>(currentTime - lastMoveTime).count();

        if (elapsedSeconds >= moveInterval) {
            object1.move();
            object2.move();
            lastMoveTime = currentTime;
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Set color to white
        SDL_RenderClear(renderer); // Clear the screen

        drawGrid(renderer); // Draw the grid

        object1.draw();
        object2.draw();

        SDL_RenderPresent(renderer); // Update the screen
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
