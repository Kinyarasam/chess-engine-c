#include "main.h"


void initializeBoard(GameState* state) {
    for (int row = 0; row < BOARD_SIZE; ++row) {
        for (int col = 0; col < BOARD_SIZE; ++col) {
            state->board[row][col].row = row;
            state->board[row][col].col = col;
            state->board[row][col].occupied = NOT_OCCUPIED;

            state->board[row][col].rect.x = col * SQUARE_SIZE;
            state->board[row][col].rect.y = row * SQUARE_SIZE;
            state->board[row][col].rect.w = SQUARE_SIZE;
            state->board[row][col].rect.h = SQUARE_SIZE;
        }
    }
}

SDL_Texture* loadTexture(const char* file, SDL_Renderer* renderer) {
    SDL_Surface* surface = SDL_LoadBMP(file);
    if (!surface) {
        fprintf(stderr, "Could not load image %s! SDL_Error: %s\n", file, SDL_GetError());
        return NULL;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

void loadPieceTextures(GameState* state) {
    state->white_pawn = loadTexture("assets/white_pawn.bmp", state->renderer); // white_pawn
    state->white_knight = loadTexture("assets/white_knight.bmp", state->renderer); // white_knight
    state->white_bishop = loadTexture("assets/white_bishop.bmp", state->renderer); // white_bishop
    state->white_rook = loadTexture("assets/white_rook.bmp", state->renderer); // white_rook
    state->white_queen = loadTexture("assets/white_queen.bmp", state->renderer); // white_queen
    state->white_king = loadTexture("assets/white_king.bmp", state->renderer); // white_king
    state->black_pawn = loadTexture("assets/black_pawn.bmp", state->renderer); // black_pawn
    state->black_knight = loadTexture("assets/black_knight.bmp", state->renderer); // black_knight
    state->black_bishop = loadTexture("assets/black_bishop.bmp", state->renderer); // black_bishop
    state->black_rook = loadTexture("assets/black_rook.bmp", state->renderer); // black_rook
    state->black_queen = loadTexture("assets/black_queen.bmp", state->renderer); // black_queen
    state->black_king = loadTexture("assets/black_king.bmp", state->renderer); // black_king
}

void initializePieces(GameState* state) {
    for (int col = 0; col < BOARD_SIZE; ++col) {
        state->board[1][col].piece.type = PAWN;
        state->board[1][col].piece.color = WHITE;
        state->board[1][col].piece.texture = state->white_pawn;

        state->board[6][col].piece.type = PAWN;
        state->board[6][col].piece.color = BLACK;
        state->board[6][col].piece.texture = state->black_pawn;
    }

    PieceType backRank[] = {ROOK, KNIGHT, BISHOP, QUEEN, KING, BISHOP, KNIGHT, ROOK};
    SDL_Texture* whiteTextures[] = {
        state->white_rook, state->white_knight, state->white_bishop,
        state->white_queen, state->white_king, state->white_bishop,
        state->white_knight, state->white_rook
    };
    SDL_Texture* blackTextures[] = {
        state->black_rook, state->black_knight, state->black_bishop,
        state->black_queen, state->black_king, state->black_bishop,
        state->black_knight, state->black_rook
    };

    for (int col = 0; col < BOARD_SIZE; ++col) {
        state->board[0][col].piece.type = backRank[col];
        state->board[0][col].piece.color = WHITE;
        state->board[0][col].piece.texture = whiteTextures[col];

        state->board[7][col].piece.type = backRank[col];
        state->board[7][col].piece.color = BLACK;
        state->board[7][col].piece.texture = blackTextures[col];
    }
}


void drawBoard(GameState* state) {
    SDL_Color lightSquare = {240, 217, 181, 255};
    SDL_Color darkSquare = {181, 136, 99, 255};

    for (int row = 0; row < BOARD_SIZE; ++row) {
        for (int col = 0; col < BOARD_SIZE; ++col) {
            if ((row + col) % 2 == 0) {
                SDL_SetRenderDrawColor(state->renderer, lightSquare.r, lightSquare.g, lightSquare.b, lightSquare.a);
            } else {
                SDL_SetRenderDrawColor(state->renderer, darkSquare.r, darkSquare.g, darkSquare.b, darkSquare.a);
            }
            SDL_RenderFillRect(state->renderer, &state->board[row][col].rect);

            if (state->board[row][col].piece.type != EMPTY) {
                SDL_RenderCopy(
                    state->renderer,
                    state->board[row][col].piece.texture,
                    NULL,
                    &state->board[row][col].rect
                );
            }
        }
    }
}

void handleEvents(GameState* state) {
    while (SDL_PollEvent(state->e)) {
        if (state->e->type == SDL_QUIT) state->gameIsActive = SDL_FALSE;
    }

}

/**
 * main - Entry point
 * 
 * Return: Always 0 (success)
 *         otherwise 1 (failure)
 */
int main(void) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    GameState state = {0};

    state.window = SDL_CreateWindow("Chess Engine", SDL_WINDOWPOS_CENTERED,
                                           SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH,
                                           WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (!state.window) {
        fprintf(stderr, "Window could not be created! SDL_Error %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    state.renderer = SDL_CreateRenderer(state.window, -1, SDL_RENDERER_ACCELERATED);
    if (!state.renderer) {
        fprintf(stderr, "renderer could not be created! SDL_Error %s\n", SDL_GetError());
        SDL_DestroyWindow(state.window);
        SDL_Quit();
        return 1;
    }

    state.e = malloc(sizeof(SDL_Event));
    if (!state.e) {
        fprintf(stderr, "Memory allocation for SDL_Event failed!\n");
        SDL_DestroyRenderer(state.renderer);
        SDL_DestroyWindow(state.window);
        SDL_Quit();
        return 1;
    }

    state.gameIsActive = SDL_TRUE;

    loadPieceTextures(&state);
    initializeBoard(&state);
    initializePieces(&state);

    while (state.gameIsActive) {
        handleEvents(&state);

        SDL_SetRenderDrawColor(state.renderer, 255, 255, 255, 255);
        SDL_RenderClear(state.renderer);

        drawBoard(&state);

        SDL_RenderPresent(state.renderer);

        SDL_Delay(16);
    }

    free(state.e);

    SDL_DestroyRenderer(state.renderer);
    SDL_DestroyWindow(state.window);
    SDL_Quit();
    return 0;
}