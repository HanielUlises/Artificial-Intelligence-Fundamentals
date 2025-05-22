#include "puzzle.h"
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <ctime>

int puzzleState[PUZZLE_ROWS][PUZZLE_COLS];
SDL_Texture* puzzleTiles[PUZZLE_ROWS][PUZZLE_COLS];

Board::Board(SDL_Renderer* renderer, const std::string& imagePath) : renderer(renderer), texture(nullptr), lastMoveTime(0) {
    SDL_Surface* imageSurface = IMG_Load(imagePath.c_str());
    if (!imageSurface) {
        std::cerr << "Failed to load image: " << IMG_GetError() << std::endl;
        return;
    }

    texture = SDL_CreateTextureFromSurface(renderer, imageSurface);
    int pieceWidth = imageSurface->w / 4;
    int pieceHeight = imageSurface->h / 4;

    for (int i = 0; i < PUZZLE_ROWS; ++i) {
        for (int j = 0; j < PUZZLE_COLS; ++j) {
            SDL_Rect srcRect = { j * pieceWidth, i * pieceHeight, pieceWidth, pieceHeight };
            SDL_Texture* pieceTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, pieceWidth, pieceHeight);
            SDL_SetRenderTarget(renderer, pieceTexture);
            SDL_RenderCopy(renderer, texture, &srcRect, nullptr);
            SDL_SetRenderTarget(renderer, nullptr);
            puzzleTiles[i][j] = pieceTexture;
            puzzleState[i][j] = i * PUZZLE_COLS + j; // 0 to 15, 15 is empty
        }
    }
    SDL_FreeSurface(imageSurface);
    ShufflePuzzle();
}

Board::~Board() {
    if (texture) SDL_DestroyTexture(texture);
    for (int i = 0; i < PUZZLE_ROWS; ++i) {
        for (int j = 0; j < PUZZLE_COLS; ++j) {
            if (puzzleTiles[i][j]) SDL_DestroyTexture(puzzleTiles[i][j]);
        }
    }
}

void Board::ShufflePuzzle() {
    srand(static_cast<unsigned int>(time(nullptr)));
    const int numShuffleMoves = 1000;
    int emptyRow = 3;
    int emptyCol = 3;

    for (int i = 0; i < numShuffleMoves; ++i) {
        int direction = rand() % 4;
        int newRow = emptyRow;
        int newCol = emptyCol;

        if (direction == 0 && emptyRow > 0) newRow = emptyRow - 1; // Up
        else if (direction == 1 && emptyRow < 3) newRow = emptyRow + 1; // Down
        else if (direction == 2 && emptyCol > 0) newCol = emptyCol - 1; // Left
        else if (direction == 3 && emptyCol < 3) newCol = emptyCol + 1; // Right

        std::swap(puzzleState[emptyRow][emptyCol], puzzleState[newRow][newCol]);
        emptyRow = newRow;
        emptyCol = newCol;
    }
}

void Board::FindEmptyTile(int& row, int& col) {
    for (int i = 0; i < PUZZLE_ROWS; ++i) {
        for (int j = 0; j < PUZZLE_COLS; ++j) {
            if (puzzleState[i][j] == 15) {
                row = i;
                col = j;
                return;
            }
        }
    }
}

void Board::MoveEmptySpace(Direction direction) {
    int emptyRow, emptyCol;
    FindEmptyTile(emptyRow, emptyCol);
    int newRow = emptyRow;
    int newCol = emptyCol;

    if (direction == Direction::UP && emptyRow > 0) newRow = emptyRow - 1;
    else if (direction == Direction::DOWN && emptyRow < PUZZLE_ROWS - 1) newRow = emptyRow + 1;
    else if (direction == Direction::LEFT && emptyCol > 0) newCol = emptyCol - 1;
    else if (direction == Direction::RIGHT && emptyCol < PUZZLE_COLS - 1) newCol = emptyCol + 1;

    std::swap(puzzleState[emptyRow][emptyCol], puzzleState[newRow][newCol]);
}

void Board::Draw(SDL_Renderer* renderer) {
    int tileWidth = SCREEN_WIDTH / PUZZLE_COLS;
    int tileHeight = SCREEN_HEIGHT / PUZZLE_ROWS;

    for (int i = 0; i < PUZZLE_ROWS; ++i) {
        for (int j = 0; j < PUZZLE_COLS; ++j) {
            int tileID = puzzleState[i][j];
            if (tileID != 15) {
                SDL_Texture* tileTexture = puzzleTiles[tileID / PUZZLE_COLS][tileID % PUZZLE_COLS];
                SDL_Rect destRect = { j * tileWidth, i * tileHeight, tileWidth, tileHeight };
                SDL_RenderCopy(renderer, tileTexture, nullptr, &destRect);
            }
        }
    }
}

void handleEvents(SDL_Event& e, bool& quit, Board& board) {
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) quit = true;
        else if (e.type == SDL_KEYDOWN) {
            switch (e.key.keysym.sym) {
            case SDLK_UP:
                board.MoveEmptySpace(Board::Direction::UP);
                break;
            case SDLK_DOWN:
                board.MoveEmptySpace(Board::Direction::DOWN);
                break;
            case SDLK_LEFT:
                board.MoveEmptySpace(Board::Direction::LEFT);
                break;
            case SDLK_RIGHT:
                board.MoveEmptySpace(Board::Direction::RIGHT);
                break;
            case SDLK_ESCAPE:
                quit = true;
                break;
            case SDLK_s:
                Puzzle solver;
                board.solutionPath = solver.solve(puzzleState);
                board.lastMoveTime = SDL_GetTicks();
                break;
            }
        }
    }
}

void runProgram() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << std::endl;
        return;
    }
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cerr << "IMG_Init failed: " << IMG_GetError() << std::endl;
        return;
    }

    SDL_Window* window = SDL_CreateWindow("15-Puzzle", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "SDL_CreateWindow failed: " << SDL_GetError() << std::endl;
        IMG_Quit();
        SDL_Quit();
        return;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "SDL_CreateRenderer failed: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return;
    }

    SDL_Event e;
    bool quit = false;
    Board board(renderer, "image/gatito.png");

    while (!quit) {
        handleEvents(e, quit, board);
        if (!board.solutionPath.empty() && SDL_GetTicks() - board.lastMoveTime > 500) {
            Board::Direction dir = board.solutionPath.front();
            board.solutionPath.erase(board.solutionPath.begin());
            board.MoveEmptySpace(dir);
            board.lastMoveTime = SDL_GetTicks();
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        board.Draw(renderer);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}

std::vector<Board::Direction> Puzzle::solve(const int initialState[PUZZLE_ROWS][PUZZLE_COLS]) {
    std::string initialString;
    for (int i = 0; i < PUZZLE_ROWS; ++i) {
        for (int j = 0; j < PUZZLE_COLS; ++j) {
            initialString += static_cast<char>(initialState[i][j]);
        }
    }

    std::string goal;
    for (int i = 0; i < 16; ++i) goal += static_cast<char>(i);

    std::queue<std::string> q;
    std::unordered_set<std::string> visited;
    std::unordered_map<std::string, std::pair<std::string, Board::Direction>> parent;

    q.push(initialString);
    visited.insert(initialString);
    parent[initialString] = {"", Board::Direction::UP};

    while (!q.empty()) {
        std::string current = q.front();
        q.pop();
        if (current == goal) {
            std::vector<Board::Direction> path;
            while (parent[current].first != "") {
                path.push_back(parent[current].second);
                current = parent[current].first;
            }
            std::reverse(path.begin(), path.end());
            return path;
        }

        size_t pos = current.find(static_cast<char>(15));
        int row = pos / PUZZLE_COLS;
        int col = pos % PUZZLE_COLS;

        std::vector<std::pair<int, int>> moves = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
        std::vector<Board::Direction> directions = {
            Board::Direction::UP, Board::Direction::DOWN,
            Board::Direction::LEFT, Board::Direction::RIGHT
        };

        for (int d = 0; d < 4; ++d) {
            int newRow = row + moves[d].first;
            int newCol = col + moves[d].second;
            if (newRow >= 0 && newRow < PUZZLE_ROWS && newCol >= 0 && newCol < PUZZLE_COLS) {
                std::string newState = current;
                std::swap(newState[row * PUZZLE_COLS + col], newState[newRow * PUZZLE_COLS + newCol]);
                if (visited.find(newState) == visited.end()) {
                    visited.insert(newState);
                    q.push(newState);
                    parent[newState] = {current, directions[d]};
                }
            }
        }
    }
    return {};
}