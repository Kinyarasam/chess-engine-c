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
} GameState;

#endif  /** __MAIN_H__ */
