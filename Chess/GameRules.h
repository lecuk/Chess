#ifndef GAME_RULES_H
#define GAME_RULES_H

#include "GameConstants.h"
#include "BoardEngine.h"

typedef enum MoveAttempt
{
	Success,					//OK
	SourceCellIsEmpty,			//There is no piece to move
	SourceEqualsDestination,	//Source and destination is the same cell
	PieceIsEnemy,				//It`s an enemy piece, can`t control it
	ImpossibleMove,				//Piece can't physically be moved here (for example, pawn moving backwards)
	OutOfBounds,				//Input co-ords are not inside board
	DestinationIsBlocked,		//Something blocks path to destination cell
	KingWillBeUnderCheck,		//If piece move, king will be attacked
	KingIsUnderCheck,			//King is currently in a check, should save him
	RookAlreadyMoved,			//Castling is impossible: rook already moved
	KingAlreadyMoved,			//Castling is impossible: king already moved
	CantCastleOutOfCheck,		//Castling is impossible: king is under check
	InvalidPromotionInput		//Promotion pieces should be Q, q, R, r, B, b, N, n
} MoveAttempt;

typedef enum MoveType
{
	SimpleMove, 
	Attack, 
	QueensideCastling,
	KingsideCastling,
	PromotionToQueen,
	PromotionToRook,
	PromotionToBishop,
	PromotionToKnight,
	EnPassant,
	InvalidMove
} MoveType;

typedef struct MoveData
{
	MoveAttempt attempt;
	MoveType type;
	int x1, y1, x2, y2;
	Piece pieceMoved, pieceTaken, piecePromotedTo;
} MoveData;

MoveData ValidateMovePhysically(const Board *board, DOUBLE_COORDS_PARAMS, PromotionInput input);
MoveData ValidateMove(const Board *board, DOUBLE_COORDS_PARAMS, PromotionInput input);
MoveData ValidatePromotion(const Board *board, DOUBLE_COORDS_PARAMS, PromotionInput input);
bool IsEnemyOfCell(const Board *board, DOUBLE_COORDS_PARAMS);
bool IsAllyOfCell(const Board *board, DOUBLE_COORDS_PARAMS);
bool IsCellUnderAttackOf(const Board *board, COORDS_PARAMS, const Player player);
float GetDefaultValue(const Board *board, COORDS_PARAMS);
bool IsCheckForPlayer(const Board *board, const Player player);
bool IsCheckmateForPlayer(const Board *board, const Player player);
bool IsStalemateForPlayer(const Board *board, const Player player);
Point FindKingOfPlayer(const Board *board, const Player player);
void DoMove(Board *board, MoveData data);
MoveData CreateMove(MoveAttempt attempt, MoveType type, DOUBLE_COORDS_PARAMS);

#endif