#ifndef __MAIN_H__
#define __MAIN_H__

#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 640
#define BOARD_SIZE 8
#define SQUARE_SIZE (WINDOW_WIDTH / BOARD_SIZE)
#define OCCUPIED true
#define NOT_OCCUPIED false

typedef enum {
    EMPTY, PAWN, KNIGHT, BISHOP,
    ROOK, QUEEN, KING
} PieceType;

typedef enum {
    NONE, WHITE, BLACK
} PieceColor;

typedef struct {
    PieceType type;
    PieceColor color;
    SDL_Texture* texture;
} Piece;

typedef struct {
    Piece movedPiece;
    int fromRow, fromCol;
    int toRow, toCol;
    Piece capturedPiece;
} Move;

typedef struct {
    int row;
    int col;
    int occupied;
    SDL_Rect rect;
    Piece piece;
} Cell;

typedef struct {
    Piece* selectedPiece;
    int selectedRow;
    int selectedCol;
    SDL_bool pieceSelected;
} PlayerState;

typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_bool gameIsActive;
    Cell board[BOARD_SIZE][BOARD_SIZE];
    SDL_Event *e;
    PlayerState playerState;

    PieceColor currentTurn;

    SDL_Texture* white_pawn;
    SDL_Texture* white_knight;
    SDL_Texture* white_bishop;
    SDL_Texture* white_rook;
    SDL_Texture* white_king;
    SDL_Texture* white_queen;
    SDL_Texture* black_pawn;
    SDL_Texture* black_knight;
    SDL_Texture* black_bishop;
    SDL_Texture* black_rook;
    SDL_Texture* black_king;
    SDL_Texture* black_queen;

    Move undoStack[256];
    Move redoStack[256];
    int undoIndex;
    int redoIndex;
} GameState;

void initializeBoard(GameState* state);
void initializePieces(GameState* state);
void loadPieceTextures(GameState* state);

SDL_Texture* loadTexture(const char* file, SDL_Renderer* renderer);

void handleEvents(GameState* state);
void handleMouseClick(GameState* state, int x, int y);
void movePiece(GameState* state, int fromRow, int fromCol, int toRow, int toCol);

SDL_bool isCheckMate(GameState* state, PieceColor color);
SDL_bool isKingInCheck(GameState* state, PieceColor color);

SDL_bool validateMove(GameState* state, int fromRow, int fromCol, int toRow, int toCol);
SDL_bool validateKingMove(GameState* state, int fromRow, int fromCol, int toRow, int toCol);
SDL_bool validateQueenMove(GameState* state, int fromRow, int fromCol, int toRow, int toCol);
SDL_bool validateBishopMove(GameState* state, int fromRow, int fromCol, int toRow, int toCol);
SDL_bool validateKnightMove(GameState* state, int fromRow, int fromCol, int toRow, int toCol);
SDL_bool validateRookMove(GameState* state, int fromRow, int fromCol, int toRow, int toCol);
SDL_bool validatePawnMove(GameState* state, int fromRow, int fromCol, int toRow, int toCol);

void drawBoard(GameState* state);

#endif  /** __MAIN_H__ */
