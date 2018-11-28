#ifndef GAME_CONSTANTS_H
#define GAME_CONSTANTS_H

#include "BasicTypes.h"

extern const Piece
NoPiece,
Pawn,
Knight,
Bishop,
Rook,
Queen,
King;

extern const Player
White,
Black,
UnknownPlayer;

extern const Cell
Empty,
WhitePawn,
BlackPawn,
WhiteKnight,
BlackKnight,
WhiteBishop,
BlackBishop,
WhiteRook,
BlackRook,
WhiteQueen,
BlackQueen,
WhiteKing,
BlackKing;

extern const int LEGAL_MOVES_KNIGHT_X[8];
extern const int LEGAL_MOVES_KNIGHT_Y[8];

extern const int LEGAL_VECTORS_BISHOP_X[4];
extern const int LEGAL_VECTORS_BISHOP_Y[4];

extern const int LEGAL_VECTORS_ROOK_X[4];
extern const int LEGAL_VECTORS_ROOK_Y[4];

extern const Position START_POSITION;
extern const Position TEST_POSITION;

extern const BoardEvaluation PAWN_EVALUATION;
extern const BoardEvaluation KNIGHT_EVALUATION;
extern const BoardEvaluation BISHOP_EVALUATION;

#endif