#include "agent.h"

int gridMatrix[GRID_SIZE][GRID_SIZE] = { 0 };

Agent::Agent(SDL_Renderer* renderer, int size, int gridSize, int initialX, int initialY, const std::string& imagePath) :
    renderer(renderer),
    size(size),
    gridSize(gridSize),
    x(initialX),
    y(initialY) {
    dx = (rand() % 3 - 1) * CELL_SIZE;
    dy = (rand() % 3 - 1) * CELL_SIZE;
    isMoving = false;

    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 16; j++) {
            gridMatrix[i][j] = 0;
        }
    }

    // Load the image as a texture
    SDL_Surface* imageSurface = IMG_Load(imagePath.c_str());
    if (imageSurface == nullptr) {
        std::cerr << "Failed to load image: " << IMG_GetError() << std::endl;
    }
    else {
        // Create a texture from the loaded image
        texture = SDL_CreateTextureFromSurface(renderer, imageSurface);
        SDL_FreeSurface(imageSurface); // Free the surface as it's no longer needed:)
    }
}

void Agent::move(int gridMatrix[][GRID_SIZE]) {
    if (!isMoving) {
        return; // If not moving, do nothing
    }

    // Matrix that determines the allowed movements of the object
    int movingMatrix[3][3] = {
        {0, 1, 0},
        {1, 0, 1},
        {0, 1, 0}
    };

    int randomDirection = rand() % 4; // 0: up, 1: down, 2: left, 3: right

    int new_dx = 0;
    int new_dy = 0;

    if (randomDirection == 0) { // Up
        new_dy = -CELL_SIZE;
    }
    else if (randomDirection == 1) { // Down
        new_dy = CELL_SIZE;
    }
    else if (randomDirection == 2) { // Left
        new_dx = -CELL_SIZE;
    }
    else if (randomDirection == 3) { // Right
        new_dx = CELL_SIZE;
    }

    int nextX = x + new_dx;
    int nextY = y + new_dy;

    // Calculate the grid position of the [potential] next position
    int gridX = nextX / CELL_SIZE;
    int gridY = nextY / CELL_SIZE;

    // Check if the movement is allowed based on the matrix
    if (nextX >= 0 && nextX < SCREEN_WIDTH && nextY >= 0 && nextY < SCREEN_HEIGHT) {
        if (movingMatrix[1 + new_dx / CELL_SIZE][1 + new_dy / CELL_SIZE]) {
            // Check if the intended next position is not blocked by an obstacle
            if (gridMatrix[gridX][gridY] != 1) {
                // Update position based on the allowed movement
                x = nextX;
                y = nextY;
                dx = new_dx;
                dy = new_dy;
            }
        }
    }
}

void Agent::stopMoving() {
    isMoving = !isMoving;
}

void Agent::draw() {
    // Drawing the ojects
    SDL_Rect agentRect = { x, y, size, size };
    SDL_RenderCopy(renderer, texture, nullptr, &agentRect);
}

bool Agent::isObstacle() {
    // Grid position based on the object's coordinates
    int gridX = x / CELL_SIZE;
    int gridY = y / CELL_SIZE;

    // Check if the current grid position is marked as an obstacle in the gridMatrix
    if (gridMatrix[gridX][gridY] == 1) {
        // If the current grid position is an obstacle, check the localMatrix for obstacles in the surroundings
        int** localMatrix = initMatrix();

        // Loop through the localMatrix to check for obstacles
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (localMatrix[i][j] == 1) {
                    // There is an obstacle in the localMatrix, so this position is obstructed
                    return true;
                }
            }
        }

        // If there are no obstacles in the localMatrix, the agent is not obstructed
        return false;
    }

    // If the current grid position is not an obstacle in the gridMatrix, the agent is not obstructed
    return false;
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

int* Agent::getX() {
    return &x;
}

int* Agent::getY() {
    return &y;
}

void drawGrid(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

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

void drawObstacles(SDL_Renderer* renderer, std::vector<std::pair<int, int>>& obstacles, int size, int gridMatrix[][GRID_SIZE]) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    for (const auto& obstacle : obstacles) {
        int obstacleX = obstacle.first * size;
        int obstacleY = obstacle.second * size;
        SDL_Rect obstacleRect = { obstacleX, obstacleY, size, size };
        SDL_RenderFillRect(renderer, &obstacleRect);

        // Mark the obstacle positions in gridMatrix
        int gridX = obstacleX / size;
        int gridY = obstacleY / size;
        // Updates the matrix
        gridMatrix[gridX][gridY] = 1;
    }
}

void handleMouseClick(SDL_Event& e, std::vector<std::pair<int, int>>& obstacles) {
    if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
        int mouseX = e.button.x;
        int mouseY = e.button.y;

        // Grid position based on mouse click coordinates
        int gridX = mouseX / CELL_SIZE;
        int gridY = mouseY / CELL_SIZE;

        obstacles.push_back(std::make_pair(gridX, gridY));
    }
}


void handleEvents(SDL_Event& e, std::vector<std::pair<int, int>>& obstacles, Agent& object1, Agent& object2, bool& quit) {
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
        else if (e.type == SDL_MOUSEBUTTONDOWN) {
            handleMouseClick(e, obstacles);
        }
    }
}

void runProgram() {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Reactive Agents", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (!IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) {
        std::cout << "Failed to initialize image" << IMG_GetError() <<std::endl;
    }
    std::vector<std::pair<int, int>> obstacles; 
    bool quit = false;  
    SDL_Event e;

    srand(static_cast<unsigned>(time(nullptr)));

    // Specifies the initial positions for object1 and object2
    int initialX1 = (rand() % 2 + 1) * 100;
    int initialY1 = (rand() % 2 + 1) * 100;
    int initialX2 = (rand() % 3 + 1) * 100;
    int initialY2 = (rand() % 3 + 1) * 100;

    Agent object1(renderer, CELL_SIZE, GRID_SIZE, initialX1, initialY1, "images/agente1.png"); // Load agent1.png
    Agent object2(renderer, CELL_SIZE, GRID_SIZE, initialX2, initialY2, "images/agente2.png"); // Load agent2.png

    auto lastMoveTime = std::chrono::high_resolution_clock::now();
    const double moveInterval = 0.1; // Move every [amount] of secondsss

    while (!quit) {
        handleEvents(e, obstacles, object1, object2, quit);

        auto currentTime = std::chrono::high_resolution_clock::now();
        double elapsedSeconds = std::chrono::duration<double>(currentTime - lastMoveTime).count();

        if (elapsedSeconds >= moveInterval) {
            object1.move(gridMatrix);
            object2.move(gridMatrix);
            lastMoveTime = currentTime;
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Background color
        SDL_RenderClear(renderer); // Clear the screen

        drawGrid(renderer); // Draw the grid

        // Draw obstacles
        drawObstacles(renderer, obstacles, CELL_SIZE, gridMatrix);

        object1.draw();
        object2.draw();

        SDL_RenderPresent(renderer); // Update the screen

        SDL_Delay(10);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
