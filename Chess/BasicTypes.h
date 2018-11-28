#ifndef BASIC_TYPES_H
#define BASIC_TYPES_H

#include <stdbool.h>

#define BOARD_SIZE 8
#define MAX_MOVES 1000

typedef unsigned short Piece;
typedef short Player;
typedef short Cell;

typedef Cell Position[BOARD_SIZE][BOARD_SIZE];
typedef double BoardEvaluation[BOARD_SIZE][BOARD_SIZE];

typedef enum PromotionSelection
{
	SelectQueen, SelectRook, SelectBishop, SelectKnight, InvalidSelection
} PromotionSelection;

typedef PromotionSelection(*PromotionInput)();

typedef struct Point
{
	int x, y;
} Point;

typedef struct Move
{
	int x1, y1, x2, y2;
} Move;

#define COORDS_PARAMS const int x, const int y
#define DOUBLE_COORDS_PARAMS const int x1, const int y1, const int x2, const int y2

#endif 
