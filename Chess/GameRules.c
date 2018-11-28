#include "GameRules.h"

static PromotionSelection _curPromotion = InvalidSelection;
static PromotionSelection _getAllPossiblePromotions()
{
	if (_curPromotion == SelectQueen) _curPromotion = SelectKnight;
	else _curPromotion = SelectQueen;
	return _curPromotion;
}

MoveData ValidateMovePhysically(const Board *board, DOUBLE_COORDS_PARAMS, PromotionInput input)
{
	Cell cell = GetCell(board, x1, y1);
	Piece piece = GetPiece(board, x1, y1);
	Player player = GetPlayer(board, x1, y1);
	Player enemy = EnemyOf(player);
	int backRank = (player == White) ? 0 : 7;
	int enemyBackRank = (player == White) ? 7 : 0;
	int pawnRank = (player == White) ? 1 : 6;
	int enemyPawnRank = (player == White) ? 6 : 1;
	int enPassantRank = (player == White) ? 4 : 3;

	if (IsEmpty(board, x1, y1)) return CreateMove(SourceCellIsEmpty, InvalidMove, x1, y1, x2, y2);
	if (IsSameCell(x1, y1, x2, y2)) return CreateMove(SourceEqualsDestination, InvalidMove, x1, y1, x2, y2);
	if (!IsInBounds(x1, y1) || !IsInBounds(x2, y2)) return CreateMove(OutOfBounds, InvalidMove, x1, y1, x2, y2);
	if (IsAllyOfCell(board, x1, y1, x2, y2)) return CreateMove(DestinationIsBlocked, InvalidMove, x1, y1, x2, y2);

	switch (piece)
	{
		//pawn
	case 1:
		//simple move
		if (x1 == x2) 
		{
			if ((y2 - y1 == 2 * player && y1 == pawnRank && IsEmpty(board, x1, y1 + player)) || y2 - y1 == player)
			{
				if (IsEmpty(board, x1, y2))
				{
					if (y2 == enemyBackRank)
					{
						return ValidatePromotion(board, x1, y1, x2, y2, input);
					}
					return CreateMove(Success, SimpleMove, x1, y1, x2, y2);
				}
				return CreateMove(DestinationIsBlocked, InvalidMove, x1, y1, x2, y2);
			}
		}
		//attack
		if (abs(x1 - x2) == 1 && y1 + player == y2)
		{
			if (IsEnemyOfCell(board, x1, y1, x2, y2))
			{
				if (y2 == enemyBackRank)
				{
					return ValidatePromotion(board, x1, y1, x2, y2, input);
				}
				return CreateMove(Success, Attack, x1, y1, x2, y2);
			}

			if (y1 == enPassantRank && IsEnemyOfCell(board, x1, y1, x2, enPassantRank) &&
				GetPiece(board, x2, enPassantRank) == Pawn &&
				board->lastMove.x1 == x2 && board->lastMove.y1 == enemyPawnRank &&
				board->lastMove.x2 == x2 && board->lastMove.y2 == enPassantRank)
			{
				return CreateMove(Success, EnPassant, x1, y1, x2, y2);
			}
		}
		return CreateMove(ImpossibleMove, InvalidMove, x1, y1, x2, y2);

		//knight
	case 2:
		for (int i = 0; i < 8; i++)
		{
			if (x2 - x1 == LEGAL_MOVES_KNIGHT_X[i] && y2 - y1 == LEGAL_MOVES_KNIGHT_Y[i])
			{
				if (IsEmpty(board, x2, y2)) 
					return CreateMove(Success, SimpleMove, x1, y1, x2, y2);

				return CreateMove(Success, Attack, x1, y1, x2, y2);
			}
		}
		return CreateMove(ImpossibleMove, InvalidMove, x1, y1, x2, y2);

		//bishop
	case 3:
		if (IsSameDiagonal(x1, y1, x2, y2))
		{
			for (int i = 0; i < 4; i++)
			{
				int vx = x1 + LEGAL_VECTORS_BISHOP_X[i];
				int vy = y1 + LEGAL_VECTORS_BISHOP_Y[i];
				while (IsInBounds(vx, vy))
				{
					if (x2 == vx && y2 == vy)
					{
						if (IsEmpty(board, x2, y2)) return CreateMove(Success, SimpleMove, x1, y1, x2, y2);
						else return CreateMove(Success, Attack, x1, y1, x2, y2);
					}
					else if (!IsEmpty(board, vx, vy)) break;
					vx += LEGAL_VECTORS_BISHOP_X[i];
					vy += LEGAL_VECTORS_BISHOP_Y[i];
				}
			}
			return CreateMove(DestinationIsBlocked, InvalidMove, x1, y1, x2, y2);
		}
		return CreateMove(ImpossibleMove, InvalidMove, x1, y1, x2, y2);

		//rook
	case 4:
		if (IsSameLine(x1, y1, x2, y2))
		{
			for (int i = 0; i < 4; i++)
			{
				int vx = x1 + LEGAL_VECTORS_ROOK_X[i];
				int vy = y1 + LEGAL_VECTORS_ROOK_Y[i];
				while (IsInBounds(vx, vy))
				{
					if (x2 == vx && y2 == vy)
					{
						if (IsEmpty(board, x2, y2)) return CreateMove(Success, SimpleMove, x1, y1, x2, y2);
						else return CreateMove(Success, Attack, x1, y1, x2, y2);
					}
					else if (!IsEmpty(board, vx, vy)) break;
					vx += LEGAL_VECTORS_ROOK_X[i];
					vy += LEGAL_VECTORS_ROOK_Y[i];
				}
			}
			return CreateMove(DestinationIsBlocked, InvalidMove, x1, y1, x2, y2);
		}
		return CreateMove(ImpossibleMove, InvalidMove, x1, y1, x2, y2);

		//queen
	case 5:
		if (IsSameDiagonal(x1, y1, x2, y2))
		{
			for (int i = 0; i < 4; i++)
			{
				int vx = x1 + LEGAL_VECTORS_BISHOP_X[i];
				int vy = y1 + LEGAL_VECTORS_BISHOP_Y[i];
				while (IsInBounds(vx, vy))
				{
					if (x2 == vx && y2 == vy)
					{
						if (IsEmpty(board, x2, y2)) return CreateMove(Success, SimpleMove, x1, y1, x2, y2);
						else return CreateMove(Success, Attack, x1, y1, x2, y2);
					}
					else if (!IsEmpty(board, vx, vy)) break;
					vx += LEGAL_VECTORS_BISHOP_X[i];
					vy += LEGAL_VECTORS_BISHOP_Y[i];
				}
			}
			return CreateMove(DestinationIsBlocked, InvalidMove, x1, y1, x2, y2);
		}
		else if (IsSameLine(x1, y1, x2, y2))
		{
			for (int i = 0; i < 4; i++)
			{
				int vx = x1 + LEGAL_VECTORS_ROOK_X[i];
				int vy = y1 + LEGAL_VECTORS_ROOK_Y[i];
				while (IsInBounds(vx, vy))
				{
					if (x2 == vx && y2 == vy)
					{
						if (IsEmpty(board, x2, y2)) return CreateMove(Success, SimpleMove, x1, y1, x2, y2);
						else return CreateMove(Success, Attack, x1, y1, x2, y2);
					}
					else if (!IsEmpty(board, vx, vy)) break;
					vx += LEGAL_VECTORS_ROOK_X[i];
					vy += LEGAL_VECTORS_ROOK_Y[i];
				}
			}
			return CreateMove(DestinationIsBlocked, InvalidMove, x1, y1, x2, y2);
		}
		return CreateMove(ImpossibleMove, InvalidMove, x1, y1, x2, y2);

		//king
	case 6:
		if ((abs(x2 - x1) <= 1) && (abs(y2 - y1) <= 1))
		{
			if (IsEmpty(board, x2, y2)) return CreateMove(Success, SimpleMove, x1, y1, x2, y2);
			else return CreateMove(Success, Attack, x1, y1, x2, y2);
		}
		else
		{
			if (y1 == backRank && y2 == backRank)
			{
				if ((player == Black && board->blackKingMoved) || (player == White && board->whiteKingMoved))
					return CreateMove(KingAlreadyMoved, InvalidMove, x1, y1, x2, y2);

				if (x2 == 2)
				{
					if ((player == Black && board->blackQueensideRookMoved) || (player == White && board->whiteQueensideRookMoved))
						return CreateMove(RookAlreadyMoved, InvalidMove, x1, y1, x2, y2);

					if (IsCheckForPlayer(board, player))
						return CreateMove(CantCastleOutOfCheck, InvalidMove, x1, y1, x2, y2);

					if (IsCellUnderAttackOf(board, 2, y1, enemy) || IsCellUnderAttackOf(board, 3, y1, enemy) ||
						!IsEmpty(board, 1, y1) || !IsEmpty(board, 2, y1) || !IsEmpty(board, 3, y1))
						return CreateMove(DestinationIsBlocked, InvalidMove, x1, y1, x2, y2);

					return CreateMove(Success, QueensideCastling, x1, y1, x2, y2);
				}
				if (x2 == 6)
				{
					if ((player == Black && board->blackKingsideRookMoved) || (player == White && board->whiteKingsideRookMoved))
						return CreateMove(RookAlreadyMoved, InvalidMove, x1, y1, x2, y2);

					if (IsCheckForPlayer(board, player))
						return CreateMove(CantCastleOutOfCheck, InvalidMove, x1, y1, x2, y2);

					if (IsCellUnderAttackOf(board, 5, y1, enemy) || IsCellUnderAttackOf(board, 6, y1, enemy) ||
						!IsEmpty(board, 5, y1) || !IsEmpty(board, 6, y1))
						return CreateMove(DestinationIsBlocked, InvalidMove, x1, y1, x2, y2);

					return CreateMove(Success, KingsideCastling, x1, y1, x2, y2);
				}
			}
		}
	}
	return CreateMove(ImpossibleMove, InvalidMove, x1, y1, x2, y2);
}

MoveData ValidateMove(const Board *board, DOUBLE_COORDS_PARAMS, PromotionInput input)
{
	Cell cell = GetCell(board, x1, y1);
	Piece piece = GetPiece(board, x1, y1);
	Player player = GetPlayer(board, x1, y1);
	
	MoveData data = ValidateMovePhysically(board, x1, y1, x2, y2, input);
	if (data.attempt != Success) return data;

	data.pieceMoved = GetPiece(board, data.x1, data.y1);

	START_TEST(board->currentPosition)
	{
		if (IsCheckForPlayer(testBoard, player))
		{
			DoMove(testBoard, data);
			if (IsCheckForPlayer(testBoard, player))
			{
				data.attempt = KingIsUnderCheck;
				data.type = InvalidMove;
			}
		}
		else
		{
			DoMove(testBoard, data);
			if (IsCheckForPlayer(testBoard, player))
			{
				data.attempt = KingWillBeUnderCheck;
				data.type = InvalidMove;
			}
		}
	}
	END_TEST;

	return data;
}

MoveData ValidatePromotion(const Board *board, DOUBLE_COORDS_PARAMS, PromotionInput input)
{
	MoveData data;
	switch (input())
	{
	case SelectQueen:
		data = CreateMove(Success, PromotionToQueen, x1, y1, x2, y2);
		data.piecePromotedTo = Queen;
		break;
	case SelectRook:
		data = CreateMove(Success, PromotionToRook, x1, y1, x2, y2);
		data.piecePromotedTo = Rook;
		break;
	case SelectBishop:
		data = CreateMove(Success, PromotionToBishop, x1, y1, x2, y2);
		data.piecePromotedTo = Bishop;
		break;
	case SelectKnight:
		data = CreateMove(Success, PromotionToKnight, x1, y1, x2, y2);
		data.piecePromotedTo = Knight;
		break;
	case InvalidSelection:
		data = CreateMove(InvalidPromotionInput, InvalidMove, x1, y1, x2, y2);
	}
	return data;
}

bool IsEnemyOfCell(const Board *board, DOUBLE_COORDS_PARAMS)
{
	return (!IsEmpty(board, x1, y1) && !IsEmpty(board, x2, y2)) && (GetPlayer(board, x1, y1) != GetPlayer(board, x2, y2));
}

bool IsAllyOfCell(const Board *board, DOUBLE_COORDS_PARAMS)
{
	return (!IsEmpty(board, x1, y1) && !IsEmpty(board, x2, y2)) && (GetPlayer(board, x1, y1) == GetPlayer(board, x2, y2));
}

bool IsCellUnderAttackOf(const Board *board, COORDS_PARAMS, Player player)
{
	ITERATE_PIECES(board, player)
	{
		if (ValidateMovePhysically(board, xp, yp, x, y, _getAllPossiblePromotions).attempt == Success) return true;
	}
	return false;
}

float GetDefaultValue(const Board *board, COORDS_PARAMS)
{
	int value;
	switch (GetPiece(board, x, y))
	{
	case 1: value = 1; break;
	case 2: value = 3; break;
	case 3: value = 3; break;
	case 4: value = 5; break;
	case 5: value = 9; break;
	case 6: value = 1000; break;
	default: value = 0;
	}
	return (float)value * GetPlayer(board, x, y);
}

bool IsCheckForPlayer(const Board *board, const Player player)
{
	Point kingPos = FindKingOfPlayer(board, player);

	return IsCellUnderAttackOf(board, kingPos.x, kingPos.y, EnemyOf(player));
}

bool IsCheckmateForPlayer(const Board *board, const Player player)
{
	if (!IsCheckForPlayer(board, player)) return false;

	ITERATE_PIECES(board, player)
	{
		ITERATE_MOVES(board, xp, yp, _getAllPossiblePromotions) //if any move is possible, then there is no checkmate
		{
			return false;
		}
	}
	return true;
}

bool IsStalemateForPlayer(const Board *board, const Player player)
{
	if (IsCheckForPlayer(board, player)) return false;

	ITERATE_PIECES(board, player)
	{
		ITERATE_MOVES(board, xp, yp, _getAllPossiblePromotions) //if any move is possible, then there is no stalemate
		{
			return false;
		}
	}
	return true;
}

Point FindKingOfPlayer(const Board *board, const Player player)
{
	Point kingPos;
	ITERATE_PIECES(board, player)
	{
		if (GetPiece(board, xp, yp) == King)
		{
			kingPos.x = xp;
			kingPos.y = yp;
			return kingPos;
		}
	}
	kingPos.x = -1;
	kingPos.y = -1;
	return kingPos;
}

void DoMove(Board *board, MoveData data)
{
	Player player = GetPlayer(board, data.x1, data.y1);
	if (data.attempt == Success)
	{
		switch (data.type)
		{
		case SimpleMove:
		case Attack:
			if (data.x1 == 0 && data.y1 == 0) board->whiteQueensideRookMoved = true;
			if (data.x1 == 7 && data.y1 == 0) board->whiteKingsideRookMoved = true;
			if (data.x1 == 0 && data.y1 == 7) board->blackQueensideRookMoved = true;
			if (data.x1 == 7 && data.y1 == 7) board->blackKingsideRookMoved = true;
			MoveCell(board, X1Y1X2Y2(data));
			break;
		case QueensideCastling:
			if (player == White)
			{
				board->whiteKingMoved = true;
				board->whiteQueensideRookMoved = true;
			}
			else
			{
				board->blackKingMoved = true;
				board->blackQueensideRookMoved = true;
			}
			MoveCell(board, X1Y1X2Y2(data));
			MoveCell(board, 0, data.y1, 3, data.y2); //move left rook
			break;
		case KingsideCastling:
			if (player == White)
			{
				board->whiteKingMoved = true;
				board->whiteKingsideRookMoved = true;
			}
			else
			{
				board->blackKingMoved = true;
				board->blackKingsideRookMoved = true;
			}
			MoveCell(board, X1Y1X2Y2(data));
			MoveCell(board, 7, data.y1, 5, data.y2); //move right rook
			break;
		case PromotionToQueen:
			MoveCell(board, X1Y1X2Y2(data));
			SetCell(board, data.x2, data.y2, Queen * GetPlayer(board, data.x2, data.y2)); //promote to queen
			data.piecePromotedTo = GetPiece(board, data.x2, data.y2);
			break;
		case PromotionToRook:
			MoveCell(board, X1Y1X2Y2(data));
			SetCell(board, data.x2, data.y2, Rook * GetPlayer(board, data.x2, data.y2)); //promote to rook
			data.piecePromotedTo = GetPiece(board, data.x2, data.y2);
			break;
		case PromotionToBishop:
			MoveCell(board, X1Y1X2Y2(data));
			SetCell(board, data.x2, data.y2, Bishop * GetPlayer(board, data.x2, data.y2)); //promote to bishop
			data.piecePromotedTo = GetPiece(board, data.x2, data.y2);
			break;
		case PromotionToKnight:
			MoveCell(board, X1Y1X2Y2(data));
			SetCell(board, data.x2, data.y2, Knight * GetPlayer(board, data.x2, data.y2));  //promote to knight
			data.piecePromotedTo = GetPiece(board, data.x2, data.y2);
			break;
		case EnPassant:
			MoveCell(board, X1Y1X2Y2(data));
			SetCell(board, data.x2, data.y1, Empty); //remove passed pawn
			break;
		}
	}
	SetLastMove(board, data.x1, data.y1, data.x2, data.y2);
}

MoveData CreateMove(MoveAttempt attempt, MoveType type, DOUBLE_COORDS_PARAMS)
{
	MoveData data;
	data.attempt = attempt; 
	data.type = type; 
	data.x1 = x1;
	data.y1 = y1;
	data.x2 = x2;
	data.y2 = y2;
	return data;
}