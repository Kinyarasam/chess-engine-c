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
    SDL_Color selectedSquare = {255, 255, 0, 255};
    SDL_Color possibleMoveSquare = {173, 216, 230, 255};
    SDL_Color checkSquare = {255, 0, 0, 255};
    SDL_Color capturableSquare = {255, 69, 0, 255};

    for (int row = 0; row < BOARD_SIZE; ++row) {
        for (int col = 0; col < BOARD_SIZE; ++col) {
            SDL_Color squareColor = ((row + col) % 2 == 0) ? lightSquare : darkSquare;

            if (state->playerState.pieceSelected && state->playerState.selectedRow == row && state->playerState.selectedCol == col) {
                squareColor = selectedSquare;
            }

            if (state->playerState.pieceSelected && validateMove(state, state->playerState.selectedRow, state->playerState.selectedCol, row, col)) {
                if (state->board[row][col].piece.type != EMPTY && state->board[row][col].piece.color != state->playerState.selectedPiece->color) {
                    squareColor = capturableSquare;
                } else {
                    squareColor = possibleMoveSquare;
                }
            }

            if (state->board[row][col].piece.type == KING && isKingInCheck(state, state->board[row][col].piece.color)) {
                squareColor = checkSquare;
            }

            SDL_SetRenderDrawColor(state->renderer, squareColor.r, squareColor.g, squareColor.b, squareColor.a);
            SDL_RenderFillRect(state->renderer, &state->board[row][col].rect);
            SDL_SetRenderDrawColor(state->renderer, 255, 255, 255, 255);
            SDL_RenderDrawRect(state->renderer, &state->board[row][col].rect);

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

SDL_bool validatePawnMove(GameState* state, int fromRow, int fromCol, int toRow, int toCol) {
    Piece* piece = &state->board[fromRow][fromCol].piece;
    int direction = (piece->color == WHITE) ? 1 : -1;
    int startRow = (piece->color == WHITE) ? 1 : 6;

    if (fromCol == toCol) {
        if (toRow == fromRow + direction && state->board[toRow][toCol].piece.type == EMPTY) {
            return SDL_TRUE;
        }

        if (fromRow == startRow && toRow == fromRow + 2 * direction && state->board[toRow][toCol].piece.type == EMPTY && state->board[fromRow + direction][toCol].piece.type == EMPTY) {
            return SDL_TRUE;
        }
    }

    if (abs(fromCol - toCol) == 1 && toRow == fromRow + direction && state->board[toRow][toCol].piece.type != EMPTY && state->board[toRow][toCol].piece.color != piece->color) {
        return SDL_TRUE;
    }

    return SDL_FALSE;
}

SDL_bool validateRookMove(GameState* state, int fromRow, int fromCol, int toRow, int toCol) {
    if (fromRow != toRow && fromCol != toCol) return SDL_FALSE;
    int stepRow = 0;
    int stepCol = 0;

    if (toRow - fromRow != 0) {
        stepRow = (toRow - fromRow) / abs(toRow - fromRow);
    }
    if (toCol - fromCol != 0) {
        stepCol = (toCol - fromCol) / abs(toCol - fromCol);
    }

    for (int i = fromRow + stepRow, j = fromCol + stepCol; i != toRow || j != toCol; i += stepRow, j+= stepCol) {
        if (state->board[i][j].piece.type != EMPTY) {
            return SDL_FALSE;
        }
    }

    return SDL_TRUE;
}

SDL_bool validateKnightMove(GameState* state, int fromRow, int fromCol, int toRow, int toCol) {
    int rowDiff = abs(fromRow - toRow);
    int colDiff = abs(fromCol - toCol);

    if ((rowDiff == 2 && colDiff == 1) || (rowDiff == 1 && colDiff == 2)) {
        return SDL_TRUE;
    }
    return SDL_FALSE;
}

SDL_bool validateBishopMove(GameState* state, int fromRow, int fromCol, int toRow, int toCol) {
    if (abs(fromRow - toRow) != abs(fromCol - toCol)) return SDL_FALSE;

    int stepRow = (toRow - fromRow != 0) ? (toRow - fromRow) / abs(toRow - fromRow) : 0;
    int stepCol = (toCol - fromCol != 0) ? (toCol - fromCol) / abs(toCol - fromCol) : 0;

    for (int i = fromRow + stepRow, j = fromCol + stepCol; i != toRow || j != toCol; i += stepRow, j += stepCol) {
        if (state->board[i][j].piece.type != EMPTY) return SDL_FALSE;
    }
    return SDL_TRUE;
}

SDL_bool validateQueenMove(GameState* state, int fromRow, int fromCol, int toRow, int toCol) {
    return validateRookMove(state, fromRow, fromCol, toRow, toCol) || validateBishopMove(state, fromRow, fromCol, toRow, toCol);
}

SDL_bool validateKingMove(GameState* state, int fromRow, int fromCol, int toRow, int toCol) {
    int rowDiff = abs(fromRow - toRow);
    int colDiff = abs(fromCol - toCol);

    if (rowDiff <= 1 && colDiff <= 1) {
        return SDL_TRUE;
    }
    return SDL_FALSE;
}

SDL_bool validateMove(GameState* state, int fromRow, int fromCol, int toRow, int toCol) {
    Piece* piece = &state->board[fromRow][fromCol].piece;

    if (toRow < 0 || toRow >= BOARD_SIZE || toCol < 0 || toCol >= BOARD_SIZE) return SDL_FALSE;
    if (fromRow == toRow && fromCol == toCol) return SDL_FALSE;

    if (state->board[toRow][toCol].piece.type != EMPTY && state->board[toRow][toCol].piece.color == piece->color) {
        return SDL_FALSE;
    }

    SDL_bool validMove = SDL_FALSE;
    switch(piece->type) {
        case PAWN:
            validMove = validatePawnMove(state, fromRow, fromCol, toRow, toCol);
            break;
        case ROOK:
            validMove = validateRookMove(state, fromRow, fromCol, toRow, toCol);
            break;
        case KNIGHT:
            validMove = validateKnightMove(state, fromRow, fromCol, toRow, toCol);
            break;
        case BISHOP:
            validMove = validateBishopMove(state, fromRow, fromCol, toRow, toCol);
            break;
        case QUEEN:
            validMove = validateQueenMove(state, fromRow, fromCol, toRow, toCol);
            break;
        case KING:
            validMove = validateKingMove(state, fromRow, fromCol, toRow, toCol);
            break;
        default:
            return SDL_FALSE;
    }

    if (!validMove) return SDL_FALSE;

    Piece tempPiece = state->board[toRow][toCol].piece;
    state->board[toRow][toCol].piece = *piece;
    state->board[fromRow][fromCol].piece.type = EMPTY;
    state->board[fromRow][fromCol].piece.color = NONE;
    state->board[fromRow][fromCol].piece.texture = NULL;

    SDL_bool kingStillInCheck = isKingInCheck(state, piece->color);

    state->board[fromRow][fromCol].piece = state->board[toRow][toCol].piece;
    state->board[toRow][toCol].piece = tempPiece;

    if (kingStillInCheck) {
        return SDL_FALSE;
    }

    return SDL_TRUE;
}

void movePiece(GameState* state, int fromRow, int fromCol, int toRow, int toCol) {
    Move move;
    move.movedPiece = state->board[fromRow][fromCol].piece;
    move.fromRow = fromRow;
    move.fromCol = fromCol;
    move.toRow = toRow;
    move.toCol = toCol;
    move.capturedPiece = state->board[toRow][toCol].piece;

    state->undoStack[state->undoIndex++] = move;
    state->redoIndex = 0;

    state->board[toRow][toCol].piece = state->board[fromRow][fromCol].piece;
    state->board[toRow][toCol].occupied = OCCUPIED;

    state->board[fromRow][fromCol].piece.type = EMPTY;
    state->board[fromRow][fromCol].piece.color = NONE;
    state->board[fromRow][fromCol].piece.texture = NULL;
    state->board[fromRow][fromCol].occupied = NOT_OCCUPIED;
}

void undoMove(GameState* state) {
    if (state->undoIndex == 0) return;

    Move move = state->undoStack[--state->undoIndex];
    state->redoStack[state->redoIndex++] = move;

    state->board[move.fromRow][move.fromCol].piece = move.movedPiece;
    state->board[move.toRow][move.toCol].piece = move.capturedPiece;
}

void redoMove(GameState* state) {
    if (state->redoIndex == 0) return;

    Move move = state->redoStack[--state->redoIndex];
    state->undoStack[state->undoIndex++] = move;

    state->board[move.toRow][move.toCol].piece = move.movedPiece;
    state->board[move.fromRow][move.fromCol].piece.type = EMPTY;
    state->board[move.fromRow][move.fromCol].piece.color = NONE;
    state->board[move.fromRow][move.fromCol].piece.texture = NULL;
}

SDL_bool isKingInCheck(GameState* state, PieceColor color) {
    int kingRow = -1, kingCol = -1;

    for (int row = 0; row < BOARD_SIZE; ++row) {
        for (int col = 0; col < BOARD_SIZE; ++col) {
            Piece* piece = &state->board[row][col].piece;
            if (piece->type == KING && piece->color == color) {
                kingRow = row;
                kingCol = col;
                break;
            }
        }
    }

    for (int row = 0; row < BOARD_SIZE; ++row) {
        for (int col = 0; col < BOARD_SIZE; ++col) {
            Piece* piece = &state->board[row][col].piece;
            if (piece->color != color && piece->type != EMPTY) {
                if (validateMove(state, row, col, kingRow, kingCol)) {
                    return SDL_TRUE;
                }
            }
        }
    }
    return SDL_FALSE;
}

SDL_bool isCheckMate(GameState* state, PieceColor color) {
    if (!isKingInCheck(state, color)) {
        return SDL_FALSE;
    }

    for (int fromRow = 0; fromRow < BOARD_SIZE; ++fromRow) {
        for (int fromCol = 0; fromCol < BOARD_SIZE; ++fromCol) {
            Piece* piece = &state->board[fromRow][fromCol].piece;
            if (piece->color == color && piece->type != EMPTY) {
                for (int toRow = 0; toRow < BOARD_SIZE; ++toRow) {
                    for (int toCol = 0; toCol < BOARD_SIZE; ++toCol) {
                        if (validateMove(state, fromRow, fromCol, toRow, toCol)) {
                            Piece tempPiece = state->board[toRow][toCol].piece;
                            state->board[toRow][toCol].piece = state->board[fromRow][toCol].piece;
                            movePiece(state, fromRow, fromCol, toRow, toCol);

                            SDL_bool stillInCheck = isKingInCheck(state, color);

                            state->board[fromRow][fromCol].piece = state->board[toRow][toCol].piece;
                            state->board[toRow][toCol].piece = tempPiece;

                            if (!stillInCheck) {
                                return SDL_FALSE;
                            }
                        }
                    }
                }
            }
        }
    }

    return SDL_TRUE;
}


void handleMouseClick(GameState* state, int x, int y) {
    int col = x / SQUARE_SIZE;
    int row = y / SQUARE_SIZE;

    if (state->playerState.selectedPiece) {
        if (
            validateMove(
                state,
                state->playerState.selectedRow,
                state->playerState.selectedCol,
                row, col)
        ) {
            Piece tempPiece = state->board[row][col].piece;
            movePiece(state, state->playerState.selectedRow, state->playerState.selectedCol, row, col);

            if (isKingInCheck(state, state->currentTurn)) {
                state->board[state->playerState.selectedRow][state->playerState.selectedCol].piece = state->board[row][col].piece;
                state->board[row][col].piece = tempPiece;
            } else {
                state->currentTurn = (state->currentTurn == WHITE) ? BLACK : WHITE;

                if (isCheckMate(state, state->currentTurn)) {
                    printf("Checkmate! %s is in check!\n", (state->currentTurn == WHITE) ? "White" : "Black");
                    state->gameIsActive = SDL_FALSE;
                } else if (isKingInCheck(state, state->currentTurn)) {
                    printf("Check! %s is in checkmate!\n", (state->currentTurn == WHITE) ? "White" : "Black");
                }

            }
        }
        state->playerState.selectedPiece = NULL;
        state->playerState.pieceSelected = SDL_FALSE;
    } else {
        if (state->board[row][col].piece.type != EMPTY && state->board[row][col].piece.color == state->currentTurn) {
            state->playerState.selectedPiece = &state->board[row][col].piece;
            state->playerState.selectedRow = row;
            state->playerState.selectedCol = col;
            state->playerState.pieceSelected = SDL_TRUE;
        }
    }
}


void handleEvents(GameState* state) {
    while (SDL_PollEvent(state->e)) {
        if (state->e->type == SDL_QUIT) {
            state->gameIsActive = SDL_FALSE;
        } else if(state->e->type == SDL_MOUSEBUTTONDOWN) {
            int x, y;
            SDL_GetMouseState(&x, &y);
            handleMouseClick(state, x, y);
        } else if (state->e->type == SDL_KEYDOWN) {
            if (state->e->key.keysym.sym == SDLK_z && (SDL_GetModState() & KMOD_CTRL)) {
                // Ctrl + Z for undo
                undoMove(state);
            } else if (state->e->key.keysym.sym == SDLK_y && (SDL_GetModState() & KMOD_CTRL)) {
                // Ctrl + y for undo
                redoMove(state);
            }
        }
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
    state.currentTurn = WHITE;

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