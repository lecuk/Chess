#ifndef BOARD_ENGINE_H
#define BOARD_ENGINE_H

#include "BasicTypes.h"
#include <stdlib.h>
#include "tools.h"

typedef struct Board
{
	Position currentPosition;
	bool whiteQueensideRookMoved, whiteKingsideRookMoved, blackQueensideRookMoved, blackKingsideRookMoved;
	bool whiteKingMoved, blackKingMoved;
	Move lastMove;
} Board;


bool IsInBounds(COORDS_PARAMS);
Cell GetCell(const Board *board, COORDS_PARAMS);
Piece GetPiece(const Board *board, COORDS_PARAMS);
Player GetPlayer(const Board *board, COORDS_PARAMS);
Player EnemyOf(const Player player);
void SetCell(Board *board, COORDS_PARAMS, const Cell cell);
void SetPiece(Board *board, COORDS_PARAMS, const Piece piece);
void SetPlayer(Board *board, COORDS_PARAMS, const Player player);
void MoveCell(Board *board, DOUBLE_COORDS_PARAMS);
void SwapCells(Board *board, DOUBLE_COORDS_PARAMS);
bool IsEmpty(const Board *board, COORDS_PARAMS);
bool IsSameCell(DOUBLE_COORDS_PARAMS);
bool IsSameLine(DOUBLE_COORDS_PARAMS);
bool IsSameDiagonal(DOUBLE_COORDS_PARAMS);
void CopyToPosition(const Board *board, Position position);
void CopyFromPosition(Board *board, const Position position);
bool IsPositionEqual(const Board *board, const Position position);
Board *CreateBoardFromPosition(const Position position);
void SetLastMove(Board *board, DOUBLE_COORDS_PARAMS);

#define START_TEST(position) Board *testBoard = CreateBoardFromPosition(position); 
#define RESET_TEST(position) CopyFromPosition(testBoard, position);
#define END_TEST free(testBoard)

#define ITERATE_CELLS(i) for (int y##i = 0; y##i < BOARD_SIZE; y##i++) for (int x##i = 0; x##i < BOARD_SIZE; x##i++)
#define ITERATE_PIECES(board, player) ITERATE_CELLS(p) if (GetPlayer(board, xp, yp) == player) 
#define ITERATE_MOVES(board, x, y, input) ITERATE_CELLS(m) if (ValidateMove(board, x, y, xm, ym, input).attempt == Success) 

#define XY(point) point.x, point.y
#define X1Y1X2Y2(move) move.x1, move.y1, move.x2, move.y2

#endif