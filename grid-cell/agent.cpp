#include "agent.h"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 800;
const int GRID_SIZE = 16;
const int CELL_SIZE = SCREEN_WIDTH / GRID_SIZE;

Agent::Agent(SDL_Renderer* renderer, int size, int gridSize, int initialX, int initialY, int redColor, int blueColor, int greenColor) :
    renderer(renderer),
    size(size),
    gridSize(gridSize),
    redColor(redColor),
    blueColor(blueColor),
    greenColor(greenColor),
    x(initialX),
    y(initialY) {
    dx = (rand() % 3 - 1) * CELL_SIZE; // Random initial velocity
    dy = (rand() % 3 - 1) * CELL_SIZE;
    isMoving = true; // Object starts moving initially

    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 16; j++) {
            gridMatrix[i][j] = 0;
        }
    }
}

void Agent::moveRandomly() {
    // Matrix that determines the allowed movements of the object
    int movingMatrix[3][3] = {
        {0, 1, 0},
        {1, 0, 1},
        {0, 1 ,0}
    };
    // Matrix based on the position of a given object within the GRIDDDD
    int **posMatrix = initMatrix();

    // This gives the (posible) positions for an object, restricting the not allowed movements
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            posMatrix[i][j] *= movingMatrix[i][j];
        }
    }

    dx = posMatrix[rand() % 3 + 1][rand() % 3 + 1];
    dy = posMatrix[rand() % 3 + 1][rand() % 3 + 1];

    // Freeing the memory of the array
    for (int i = 0; i < 3; i++) {
        delete[] posMatrix[i];
    }
    delete[] posMatrix;
}

void Agent::move() {
    if (!isMoving) {
        return; // If not moving, do nothing
    }

    int nextX = x + dx;
    int nextY = y + dy;

    // Check for collision with the window boundaries
    if (nextX >= 0 && nextX < SCREEN_WIDTH && nextY >= 0 && nextY < SCREEN_HEIGHT) {
        // Next position updated
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
    isMoving = !isMoving;
}

void Agent::draw() {
    SDL_SetRenderDrawColor(renderer, redColor, blueColor, greenColor, 255);
    SDL_Rect rect = { x, y, size, size };
    SDL_RenderFillRect(renderer, &rect);
}

int** Agent::initMatrix() {
    // Local matrix that tracks the current position of a given object in the 16x16 grid
    int** localMatrix = new int* [3];

    for (int i = 0; i < 3; i++) {
        localMatrix[i] = new int[3];
        for (int j = 0; j < 3; j++) {
            // Tracking the current position
            localMatrix[i][j] = gridMatrix[x - 1 + i][y - 1 + j];
        }
    }

    return localMatrix;
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

    Agent object1(renderer, CELL_SIZE, GRID_SIZE, initialX1, initialY1, 255, 0, 0); // Red color
    Agent object2(renderer, CELL_SIZE, GRID_SIZE, initialX2, initialY2, 0, 0, 255); // Blue color

    auto lastMoveTime = std::chrono::high_resolution_clock::now();
    const double moveInterval = 0.5; // Move every [amount] of secondsss

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
                if (e.key.keysym.sym == SDLK_ESCAPE) {
                    quit = !quit;
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
