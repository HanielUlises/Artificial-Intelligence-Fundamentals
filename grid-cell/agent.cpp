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

    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            gridMatrix[i][j] = 0;
        }
    }

    SDL_Surface* imageSurface = IMG_Load(imagePath.c_str());
    if (imageSurface == nullptr) {
        std::cerr << "Failed to load image: " << IMG_GetError() << std::endl;
    }
    else {
        texture = SDL_CreateTextureFromSurface(renderer, imageSurface);
        SDL_FreeSurface(imageSurface);
    }
}

Agent::~Agent() {
    if (texture != nullptr) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }

    for (int i = 0; i < 3; i++) {
        delete localMatrix[i];
        localMatrix[i] = nullptr;
    }
}

void Agent::move(int gridMatrix[][GRID_SIZE]) {
    if (!isMoving) {
        return; // If not moving, do nothing
    }

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

    // Calculate the grid position of the intended next position
    int gridX = nextX / CELL_SIZE;
    int gridY = nextY / CELL_SIZE;

    // Check if the movement is allowed based on the gridMatrix
    if (nextX >= 0 && nextX < SCREEN_WIDTH && nextY >= 0 && nextY < SCREEN_HEIGHT) {
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

void Agent::moveTowardsShip(int shipX, int shipY, int gridMatrix[][GRID_SIZE]) {
    if (!hasSample) {
        return; // Only move towards the ship if the agent has a sample
    }

    // Calculate the direction to move towards the ship
    int dx = 0;
    int dy = 0;

    if (x < shipX) {
        dx = CELL_SIZE;
    }
    else if (x > shipX) {
        dx = -CELL_SIZE;
    }

    if (y < shipY) {
        dy = CELL_SIZE;
    }
    else if (y > shipY) {
        dy = -CELL_SIZE;
    }

    int nextX = x + dx;
    int nextY = y + dy;

    // Calculate the grid position of the intended next position
    int gridX = nextX / CELL_SIZE;
    int gridY = nextY / CELL_SIZE;

    // Check if the movement is allowed based on the gridMatrix and obstacles
    if (nextX >= 0 && nextX < SCREEN_WIDTH && nextY >= 0 && nextY < SCREEN_HEIGHT) {
        // Check if the intended next position is not blocked by an obstacle
        if (gridMatrix[gridX][gridY] != 1) {
            // Update position based on the allowed movement
            x = nextX;
            y = nextY;
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

bool Agent::gotSample() {
    return hasSample;
}

bool Agent::isSample(int gridMatrix[][GRID_SIZE]) {
    // Grid position based on the agent's coordinates
    int gridX = x / CELL_SIZE;
    int gridY = y / CELL_SIZE;

    // Check if the agent is at a sample location
    return (gridMatrix[gridX][gridY] == 2);
}

void Agent::pickSample(std::vector<std::pair<int, int>>& activeSamples, std::vector<std::pair<int, int>>& samples, int gridMatrix[][GRID_SIZE]) {
    if (!hasSample) {
        int gridX = x / CELL_SIZE;
        int gridY = y / CELL_SIZE;

        // Check if there is a sample at the agent's current position
        for (auto it = samples.begin(); it != samples.end(); ++it) {
            if (it->first == gridX && it->second == gridY) {
                // Remove the sample from the samples vector
                samples.erase(it);

                // Update the gridMatrix to indicate the sample is no longer there
                gridMatrix[gridX][gridY] = 0;

                // Add the sample to the activeSamples vector
                activeSamples.push_back(std::make_pair(gridX, gridY));

                hasSample = true;
                break;
            }
        }
    }
}
// When an agent reaches the ship, it places the sample
void Agent::dropSample() {
    if (hasSample) {
        hasSample = false; // Reset the hasSample status
    }
}

std::vector<std::pair<int, int>>& Agent::getActiveSamples() {
    return activeSamples;
}

int** Agent::initMatrix() {
    // Local matrix that tracks the current position of a given object in the 16x16 grid

    for (int i = 0; i < 3; i++) {
        localMatrix[i] = new int[3];
        for (int j = 0; j < 3; j++) {
            // Tracking the current position
            localMatrix[i][j] = gridMatrix[x - 1 + i][y - 1 + j];
        }
    }

    return localMatrix;
}

int Agent::getX() {
    return x;
}

int Agent::getY() {
    return y;
}

// Ship methods
Ship::Ship(SDL_Renderer* renderer, int size, int initialX, int initialY, const std::string& imagePath) :
    renderer(renderer),
    size(size),
    x(initialX),
    y(initialY) {

    SDL_Surface* imageSurface = IMG_Load(imagePath.c_str());
    if (imageSurface == nullptr) {
        std::cerr << "Failed to load image: " << IMG_GetError() << std::endl;
    }
    else {
        texture = SDL_CreateTextureFromSurface(renderer, imageSurface);
        SDL_FreeSurface(imageSurface);
    }
}

Ship::~Ship() {
    if (texture != nullptr) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
}

void Ship::draw() {
    // Drawing the ship
    SDL_Rect shipRect = { x, y, size, size };
    SDL_RenderCopy(renderer, texture, nullptr, &shipRect);
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
        gridMatrix[gridX][gridY] = 1;
    }
}

void initializeSamples(std::vector<std::pair<int, int>>& samples, int numSamples) {
    samples.clear(); // Remove any existing samples

    for (int i = 0; i < numSamples; i++) {
        // Random positions for the sample on the grid
        int sampleX = rand() % GRID_SIZE;
        int sampleY = rand() % GRID_SIZE;

        // Smple position in gridMatrix
        gridMatrix[sampleX][sampleY] = 2;

        // Add the sample position to the samples vector
        samples.push_back(std::make_pair(sampleX, sampleY));
    }
}

void drawSamples(SDL_Renderer* renderer, const std::vector<std::pair<int, int>>& activeSamples, int size) {
    SDL_Texture* sampleTexture = nullptr;
    SDL_Surface* imageSurface = IMG_Load("images/sample.png");

    if (imageSurface == nullptr) {
        std::cerr << "Failed to load image: " << IMG_GetError() << std::endl;
    }
    else {
        sampleTexture = SDL_CreateTextureFromSurface(renderer, imageSurface);
        SDL_FreeSurface(imageSurface);
    }

    // Draw the CURRENT samples on the screen
    for (const auto& sample : activeSamples) {
        int sampleX = sample.first * size;
        int sampleY = sample.second * size;
        SDL_Rect sampleRect = { sampleX, sampleY, size, size };
        SDL_RenderCopy(renderer, sampleTexture, nullptr, &sampleRect);
    }
}

void handleMouseClick(SDL_Event& e, std::vector<std::pair<int, int>>& obstacles, std::vector<std::pair<int, int>>& samples) {
    if (e.type == SDL_MOUSEBUTTONDOWN) {
        int mouseX = e.button.x;
        int mouseY = e.button.y;

        int gridX = mouseX / CELL_SIZE;
        int gridY = mouseY / CELL_SIZE;

        if (e.button.button == SDL_BUTTON_LEFT) {
            // Check if the clicked position is not already occupied by an obstacle or a sample
            bool isOccupied = false;
            for (const auto& obstacle : obstacles) {
                if (obstacle.first == gridX && obstacle.second == gridY) {
                    isOccupied = true;
                    break;
                }
            }

            for (const auto& sample : samples) {
                if (sample.first == gridX && sample.second == gridY) {
                    isOccupied = true;
                    break;
                }
            }

            // If the position is not occupied, add it as an obstacle
            if (!isOccupied) {
                obstacles.push_back(std::make_pair(gridX, gridY));
                // Mark the obstacle position in gridMatrix
                gridMatrix[gridX][gridY] = 1;
            }
        }
        else if (e.button.button == SDL_BUTTON_RIGHT) {
            // Remove an obstacle if right-clicked
            for (auto it = obstacles.begin(); it != obstacles.end(); ++it) {
                if (it->first == gridX && it->second == gridY) {
                    // Remove the obstacle from the obstacles vector
                    obstacles.erase(it);

                    // Update the gridMatrix to indicate the obstacle is no longer there
                    gridMatrix[gridX][gridY] = 0;

                    break;
                }
            }
        }
    }
}

void handleEvents(SDL_Event& e, std::vector<std::pair<int, int>>& obstacles, std::vector<std::pair<int, int>>& samples, Agent& object1, Agent& object2, bool& quit) {
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
            handleMouseClick(e, obstacles, samples);
        }
    }
}

void runProgram() {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Reactive Agents", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_Event e;

    if (!IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) {
        std::cout << "Failed to initialize images" << IMG_GetError() << std::endl;
    }
    int randomSampleX = (rand() % 2 + 1) * 100;
    int randomSampleY = (rand() % 2 + 1) * 100;
    // Par ordenado para la posicion (x,y) en el mapa both for obstacles and samples    
    std::vector<std::pair<int, int>> obstacles;
    std::vector<std::pair<int, int>> samples;

    bool quit = false;
 
    srand(static_cast<unsigned>(time(nullptr)));

    int initialXShip = 100;
    int initialYShip = 100;

    // Specifies the initial positions for object1 and object2
    int initialX1 = (rand() % 2 + 1) * 100;
    int initialY1 = (rand() % 2 + 1) * 100;
    int initialX2 = (rand() % 4 + 1) * 100;
    int initialY2 = (rand() % 3 + 1) * 100;

    Agent object1(renderer, CELL_SIZE, GRID_SIZE, initialX1, initialY1, "images/agente1.png");
    Agent object2(renderer, CELL_SIZE, GRID_SIZE, initialX2, initialY2, "images/agente2.png");

    Ship ship(renderer, CELL_SIZE, initialXShip, initialYShip, "images/ship.png");

    auto lastMoveTime = std::chrono::high_resolution_clock::now();
    const double moveInterval = 0.1f; // Move every [amount] of seconds

    const int numSamples = 40;
    initializeSamples(samples, numSamples);

    while (!quit) {
        handleEvents(e, obstacles, samples, object1, object2, quit);

        auto currentTime = std::chrono::high_resolution_clock::now();
        double elapsedSeconds = std::chrono::duration<double>(currentTime - lastMoveTime).count();

        if (elapsedSeconds >= moveInterval) {
            object1.move(gridMatrix);
            object2.move(gridMatrix);

            lastMoveTime = currentTime;

            if (object1.isSample(gridMatrix)) {
                if (!object1.gotSample()) { // Agent 1 doesn't have a sample
                    object1.pickSample(object1.getActiveSamples(), samples, gridMatrix);
                    std::cout << "Agent 1 picked a sample." << std::endl;
                }
                else {
                    std::cout << "Agent 1 already has a sample." << std::endl;
                }
            }
            else if (object1.gotSample()) {
                // Agent 1 has a sample, move towards the ship and drop the sample
                object1.moveTowardsShip(initialXShip, initialYShip, gridMatrix);
                if (object1.getX() == initialXShip && object1.getY() == initialYShip) {
                    object1.dropSample();
                    std::cout << "Agent 1 dropped the sample at the ship." << std::endl;
                }
            }

            if (object2.isSample(gridMatrix)) {
                if (!object2.gotSample()) { // Agent 2 doesn't have a sample
                    object2.pickSample(object2.getActiveSamples(), samples, gridMatrix);
                    std::cout << "Agent 2 picked a sample." << std::endl;
                }
                else {
                    std::cout << "Agent 2 already has a sample." << std::endl;
                }
            }
            else if (object2.gotSample()) {
                // Agent 2 has a sample, move towards the ship and drop the sample
                object2.moveTowardsShip(initialXShip, initialYShip, gridMatrix);
                if (object2.getX() == initialXShip && object2.getY() == initialYShip) {
                    object2.dropSample();
                    std::cout << "Agent 2 dropped the sample at the ship." << std::endl;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Background color
        SDL_RenderClear(renderer); // Clear the screen

        drawGrid(renderer); // Draw the grid

        // Draw obstacles
        drawObstacles(renderer, obstacles, CELL_SIZE, gridMatrix);

        // Draw samples
        drawSamples(renderer, samples, CELL_SIZE);

        ship.draw();
        
        object1.draw();
        object2.draw();

        SDL_RenderPresent(renderer); // Update the screen

        SDL_Delay(10);
        if (numSamples == 0) {
            break;
        }
    }

    obstacles.clear();
    samples.clear();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
