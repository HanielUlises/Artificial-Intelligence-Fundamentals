#include "puzzle.h"

int puzzleState[PUZZLE_ROWS][PUZZLE_COLS];
SDL_Texture* puzzleTiles[PUZZLE_ROWS][PUZZLE_COLS];

Board::Board(SDL_Renderer* renderer, const std::string& imagePath) {
    this->renderer = renderer;

    SDL_Surface* imageSurface = IMG_Load(imagePath.c_str());
    if (imageSurface == nullptr) {
        std::cerr << "Failed to load image: " << IMG_GetError() << std::endl;
    }
    else {
  
        texture = SDL_CreateTextureFromSurface(renderer, imageSurface);
        SDL_FreeSurface(imageSurface);

        int pieceWidth = imageSurface->w / 4; // Assuming a 4x4 puzzle
        int pieceHeight = imageSurface->h / 4;

        for (int i = 0; i < PUZZLE_ROWS; ++i) {
            for (int j = 0; j < PUZZLE_COLS; ++j) {
                SDL_Rect srcRect = { j * pieceWidth, i * pieceHeight, pieceWidth, pieceHeight };
                SDL_Texture* pieceTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, pieceWidth, pieceHeight);
                SDL_SetRenderTarget(renderer, pieceTexture);
                SDL_RenderCopy(renderer, texture, &srcRect, NULL);
                SDL_SetRenderTarget(renderer, nullptr);

                puzzleTiles[i][j] = pieceTexture;
                puzzleState[i][j] = i * PUZZLE_COLS + j;
            }
        }

        ShufflePuzzle();
    }
}

Board::~Board() {
	if (texture != nullptr) {
		SDL_DestroyTexture(texture);
		texture = nullptr;
	}
}

void Board::ShufflePuzzle() {
    srand(static_cast<unsigned int>(time(nullptr)));

    const int numShuffleMoves = 1000; 
    int emptyRow = 3;  // Initial position of the empty space
    int emptyCol = 3;

    for (int i = 0; i < numShuffleMoves; ++i) {
        int direction = rand() % 4;

 
        int newRow = emptyRow;
        int newCol = emptyCol;

        if (direction == 0 && emptyRow > 0) {
            newRow = emptyRow - 1; // Move up
        }
        else if (direction == 1 && emptyRow < 3) {
            newRow = emptyRow + 1; // Move down
        }
        else if (direction == 2 && emptyCol > 0) {
            newCol = emptyCol - 1; // Move left
        }
        else if (direction == 3 && emptyCol < 3) {
            newCol = emptyCol + 1; // Move right
        }

        std::swap(puzzleState[emptyRow][emptyCol], puzzleState[newRow][newCol]);

        emptyRow = newRow;
        emptyCol = newCol;
    }
}

void Board::FindEmptyTile(int& row, int& col) {
    for (int i = 0; i < PUZZLE_ROWS; ++i) {
        for (int j = 0; j < PUZZLE_COLS; ++j) {
            if (puzzleState[i][j] == 0) {
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

    if (direction == Direction::UP && emptyRow > 0) {
        newRow = emptyRow - 1;
    }
    else if (direction == Direction::DOWN && emptyRow < PUZZLE_ROWS - 1) {
        newRow = emptyRow + 1;
    }
    else if (direction == Direction::LEFT && emptyCol > 0) {
        newCol = emptyCol - 1;
    }
    else if (direction == Direction::RIGHT && emptyCol < PUZZLE_COLS - 1) {
        newCol = emptyCol + 1;
    }

    // Swaps the positions of the empty tile and the selected tile
    std::swap(puzzleState[emptyRow][emptyCol], puzzleState[newRow][newCol]);
    emptyRow = newRow;
    emptyCol = newCol;
}

void Board::Draw(SDL_Renderer* renderer) {
    int tileWidth = SCREEN_WIDTH / PUZZLE_COLS;
    int tileHeight = SCREEN_HEIGHT / PUZZLE_ROWS;

    for (int i = 0; i < PUZZLE_ROWS; ++i) {
        for (int j = 0; j < PUZZLE_COLS; ++j) {
            int tileID = puzzleState[i][j];

            if (tileID != 0) {
                SDL_Texture* tileTexture = puzzleTiles[tileID / PUZZLE_COLS][tileID % PUZZLE_COLS];

                SDL_Rect destRect = { j * tileWidth, i * tileHeight, tileWidth, tileHeight };
                SDL_RenderCopy(renderer, tileTexture, nullptr, &destRect);
            }
        }
    }
}


void handleEvents(SDL_Event& e, bool& quit, Board& board) {
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            quit = true;
        }
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
            }
            
        }
    }
}

void runProgram() {
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window* window = SDL_CreateWindow("Puzzle BFS/DFS", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	SDL_Event e;

	bool quit = false;

	Board newBoard(renderer, "image/gatito.png");
	while (!quit) {
		handleEvents(e, quit, newBoard);

		SDL_RenderClear(renderer);

		newBoard.Draw(renderer);

		SDL_RenderPresent(renderer);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
