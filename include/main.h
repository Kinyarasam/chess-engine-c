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
} Piece;

typedef struct {
    int row;
    int col;
    int occupied;
    SDL_Rect rect;
    Piece piece;
} Cell;

typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_bool gameIsActive;
    Cell board[BOARD_SIZE][BOARD_SIZE];
    SDL_Event *e;
} GameState;

#endif  /** __MAIN_H__ */
