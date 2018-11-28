#include "BoardEngine.h"

bool IsInBounds(COORDS_PARAMS)
{
	return !(x < 0 || x >= BOARD_SIZE || y < 0 || y >= BOARD_SIZE);
}

Cell GetCell(const Board *board, COORDS_PARAMS)
{
	if (IsInBounds(x, y)) return board->currentPosition[y][x];
	return 0;
}

Piece GetPiece(const Board *board, COORDS_PARAMS)
{
	return abs(GetCell(board, x, y));
	return 0;
}

Player GetPlayer(const Board *board, COORDS_PARAMS)
{
	return sign(GetCell(board, x, y));
	return 0;
}

Player EnemyOf(const Player player)
{
	return -player;
}

void SetCell(Board *board, COORDS_PARAMS, const Cell cell)
{
	if (IsInBounds(x, y)) board->currentPosition[y][x] = cell;
}

void SetPiece(Board *board, COORDS_PARAMS, const Piece piece)
{
	SetCell(board, x, y, piece * GetPlayer(board, x, y));
}

void SetPlayer(Board *board, COORDS_PARAMS, const Player player)
{
	SetCell(board, x, y, GetCell(board, x, y) * player);
}

void MoveCell(Board *board, DOUBLE_COORDS_PARAMS)
{
	if (IsInBounds(x1, y1) && IsInBounds(x2, y2))
	{
		board->currentPosition[y2][x2] = board->currentPosition[y1][x1];
		board->currentPosition[y1][x1] = 0;
	}
}

void SwapCells(Board *board, DOUBLE_COORDS_PARAMS)
{
	if (IsInBounds(x1, y1) && IsInBounds(x2, y2))
	{
		int temp = board->currentPosition[y1][x1];
		board->currentPosition[y1][x1] = board->currentPosition[y2][x2];
		board->currentPosition[y2][x2] = temp;
	}
}

bool IsEmpty(const Board *board, COORDS_PARAMS)
{
	return (!GetCell(board, x, y));
}

bool IsSameCell(DOUBLE_COORDS_PARAMS)
{
	return (x1 == x2) && (y1 == y2);
}

bool IsSameLine(DOUBLE_COORDS_PARAMS)
{
	return (x1 == x2) || (y1 == y2);
}

bool IsSameDiagonal(DOUBLE_COORDS_PARAMS)
{
	return (abs(x1 - x2) == abs(y1 - y2));
}

void CopyToPosition(const Board *board, Position position)
{
	ITERATE_CELLS(i)
	{
		position[yi][xi] = board->currentPosition[yi][xi];
	}
}

void CopyFromPosition(Board *board, const Position position)
{
	ITERATE_CELLS(i)
	{
		(board->currentPosition)[yi][xi] = position[yi][xi];
	}
}

bool IsPositionEqual(const Board *board, Position position)
{
	ITERATE_CELLS(i)
	{
		if (board->currentPosition[yi][xi] != position[yi][xi]) return false;
	}
	return true;
}

Board *CreateBoardFromPosition(const Position position)
{
	Board *board = (Board*)malloc(sizeof(Board));
	if (board)
	{
		board->whiteQueensideRookMoved = false;
		board->whiteKingsideRookMoved = false;
		board->blackQueensideRookMoved = false;
		board->blackKingsideRookMoved = false;
		board->whiteKingMoved = false;
		board->blackKingMoved = false;
		CopyFromPosition(board, position);
	}
	return board;
}

void SetLastMove(Board *board, DOUBLE_COORDS_PARAMS)
{
	Move move;
	move.x1 = x1;
	move.y1 = y1;
	move.x2 = x2;
	move.y2 = y2;
	board->lastMove = move;
}