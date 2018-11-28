#include "Game.h"

Point ReadChessNotationPosition(char *getInput)
{
	Point position;

	position.x = getInput[0] - 'a';
	position.y = getInput[1] - '1';

	return position;
}

void WriteMoveToBuffer(MoveText buffer, MoveData data, bool check, bool checkmate) 
{
	switch (data.type)
	{
	case SimpleMove:
		if (data.pieceMoved == Pawn) sprintf(buffer, "%c%d", data.x2 + 'a', data.y2 + 1);
		else sprintf(buffer, "%s%c%d", PieceToChar(data.pieceMoved), data.x2 + 'a', data.y2 + 1);
		break;
	case Attack:
		if (data.pieceMoved == Pawn) sprintf(buffer, "%cx%c%d", data.x1 + 'a', data.x2 + 'a', data.y2 + 1);
		else sprintf(buffer, "%sx%c%d", PieceToChar(data.pieceMoved), data.x2 + 'a', data.y2 + 1);
		break;
	case EnPassant:
		sprintf(buffer, "%cx%c%de.p.", data.x1 + 'a', data.x2 + 'a', data.y2 + 1);
		break;
	case QueensideCastling:
		sprintf(buffer, "0-0-0");
		break;
	case KingsideCastling:
		sprintf(buffer, "0-0");
		break;
	case PromotionToQueen:
	case PromotionToRook:
	case PromotionToBishop:
	case PromotionToKnight:
		sprintf(buffer, "%c%d=%s", data.x2 + 'a', data.y2 + 1, PieceToChar(data.piecePromotedTo));
		break;
	}
	if (checkmate)
	{
		strcat(buffer, "#");
	}
	else
	{
		if (check)
		{
			strcat(buffer, "+");
		}
	}
}

bool ReadMove(char *getInput, Point *src, Point *dest)
{
	if (strlen(getInput) != 4)
	{
		return false;
	}

	char inputSrc[3] = { getInput[0], getInput[1] };
	char inputDest[3] = { getInput[2], getInput[3] };

	Point srcVal = ReadChessNotationPosition(inputSrc);
	Point destVal = ReadChessNotationPosition(inputDest);

	if (!IsInBounds(XY(srcVal)) || !IsInBounds(XY(destVal))) return false;

	*src = srcVal;
	*dest = destVal;

	return true;
}

PromotionSelection GetPromotionInput()
{
	printf(C_YELLOW "Promotion!" C_RESET " Select, to which piece should promote your pawn?\n");
	char c;
	getchar(); //this is for trailing \n
	c = getchar();
	switch (c)
	{
	case 'Q':
	case 'q':
		return SelectQueen;
	case 'R':
	case 'r':
		return SelectRook;
	case 'B':
	case 'b':
		return SelectBishop;
	case 'N':
	case 'n':
		return SelectKnight;
	default:
		return InvalidSelection;
	}
}

MoveData TryMove(Game *game, DOUBLE_COORDS_PARAMS)
{
	MoveData data;
	if (!IsEmpty(game->mainBoard, x1, y1) && GetPlayer(game->mainBoard, x1, y1) != game->playerTurn)
		data = CreateMove(PieceIsEnemy, InvalidMove, x1, y1, x2, y2);
	else data = ValidateMove(game->mainBoard, x1, y1, x2, y2, GetPromotionInput);

	switch (data.attempt)
	{
	case Success:
		switch (data.type)
		{
		case SimpleMove:
			printf(C_BOLD C_CYAN "%s" C_RESET " moved to new location.\n", PieceToName(GetPiece(game->mainBoard, x1, y1)));
			break;
		case Attack:
			printf(C_BOLD C_CYAN "%s" C_RESET " attacked opponents " C_BOLD C_CYAN "%s" C_RESET ".\n",
				PieceToName(GetPiece(game->mainBoard, x1, y1)), PieceToName(GetPiece(game->mainBoard, x2, y2)));
			break;
		case QueensideCastling:
			printf(C_BOLD C_CYAN "%s" C_RESET " castled with queenside " C_BOLD C_CYAN "%s" C_RESET ".\n",
				PieceToName(GetPiece(game->mainBoard, x1, y1)), PieceToName(Rook));
			break;
		case KingsideCastling:
			printf(C_BOLD C_CYAN "%s" C_RESET " castled with kingside " C_BOLD C_CYAN "%s" C_RESET ".\n",
				PieceToName(GetPiece(game->mainBoard, x1, y1)), PieceToName(Rook));
			break;
		case EnPassant:
			printf(C_BOLD C_CYAN "%s" C_RESET " took enemy " C_BOLD C_CYAN "%s" C_RESET " by en passant.\n",
				PieceToName(GetPiece(game->mainBoard, x1, y1)), PieceToName(Pawn));
			break;
		case PromotionToQueen:
		case PromotionToRook:
		case PromotionToBishop:
		case PromotionToKnight:
			printf(C_BOLD C_CYAN "%s" C_RESET " promoted to a " C_BOLD C_CYAN "%s" C_RESET ".\n",
				PieceToName(GetPiece(game->mainBoard, x1, y1)), PieceToName(data.piecePromotedTo));
			break;
		}
		DoMove(game->mainBoard, data);
		break;
	case SourceCellIsEmpty:
		printf(C_BOLD C_RED "Invalid move! " C_RESET "There is no piece to move.\n");
		break;
	case SourceEqualsDestination:
		printf(C_BOLD C_RED "Invalid move! " C_RESET "You typed same coordinate twice.\n");
		break;
	case PieceIsEnemy:
		printf(C_BOLD C_RED "Invalid move! " C_RESET "Can`t move your opponents pieces.\n");
		break;
	case ImpossibleMove:
		printf(C_BOLD C_RED "Invalid move! " C_RESET "Piece can't move here because of the rules of chess.\n");
		break;
	case OutOfBounds:
		printf(C_BOLD C_RED "Invalid move! " C_RESET "Input coordinates are out of bounds.\n");
		break;
	case DestinationIsBlocked:
		printf(C_BOLD C_RED "Invalid move! " C_RESET "Some piece blocks path to the destination point.\n");
		break;
	case KingWillBeUnderCheck:
		printf(C_BOLD C_RED "Invalid move! " C_RESET "If piece move, king will be attacked.\n");
		break;
	case KingIsUnderCheck:
		printf(C_BOLD C_RED "Invalid move! " C_RESET "Save your king! He is under attack!\n");
		break;
	case KingAlreadyMoved:
		printf(C_BOLD C_RED "Invalid move! " C_RESET "King already moved, castling is impossible.\n");
		break;
	case RookAlreadyMoved:
		printf(C_BOLD C_RED "Invalid move! " C_RESET "Rook already moved, castling is impossible.\n");
		break;
	case CantCastleOutOfCheck:
		printf(C_BOLD C_RED "Invalid move! " C_RESET "You can`t castle out of check.\n");
		break;
	case InvalidPromotionInput:
		printf(C_BOLD C_RED "Invalid move! " C_RESET "Allowed promotion inputs: Q, q, R, r, B, b, N, n.\n");
		break;
	default:
		break;
	}
	return data;
}

char *PlayerToName(Player player)
{
	switch (player)
	{
	case -1: return "Black";
	case 1: return "White";
	default: return "Unknown";
	}
}

void MoveToText(MoveText text, DOUBLE_COORDS_PARAMS)
{
	char cx1 = 'a' + x1;
	char cy1 = '1' + y1;
	char cx2 = 'a' + x2;
	char cy2 = '1' + y2;

	text[0] = x1;
	text[1] = y1;
	text[2] = x2;
	text[3] = y2;
}

char *PieceToChar(Piece piece)
{
	switch (piece)
	{
	case 0: return " ";
	case 1: return "P";
	case 2: return "N";
	case 3: return "B";
	case 4: return "R";
	case 5: return "Q";
	case 6: return "K";
	default: return "?";
	}
}

char *PieceToName(Piece piece)
{
	switch (abs(piece))
	{
	case 0: return "Empty";
	case 1: return "Pawn";
	case 2: return "Knight";
	case 3: return "Bishop";
	case 4: return "Rook";
	case 5: return "Queen";
	case 6: return "King";
	default: return "?";
	}
}

Game *StartNewGame()
{
	return StartGameFromPosition(START_POSITION);
}

Game *StartGameFromPosition(const Position position)
{
	Game *game = (Game*)malloc(sizeof(Game));
	game->currentMove = 0;
	game->playerTurn = White;
	game->mainBoard = CreateBoardFromPosition(position);
	game->state = WaitingForFirstMove;
	game->result = UnknownResult;
	game->blackComputerActive = false;
	game->whiteComputerActive = false;
	return game;
}

void PlayGame(Game *game)
{
	DrawGameBoard(game);
	while (game->state == Playing || game->state == WaitingForFirstMove)
	{
		//CopyToPosition(game->mainBoard, game->positions[game->currentMove]);
		if ((game->playerTurn == Black && !game->blackComputerActive) || game->playerTurn == White && !game->whiteComputerActive)
		{
			if (game->playerTurn == Black) printf(C_DGRAY); else printf(C_WHITE);
			printf(C_UNDERLINE "%s" C_RESET " player, your turn.\n", PlayerToName(game->playerTurn));

		inputLabel:
			printf("Enter your move or command: \n" C_GREEN);
			char getInput[10];
			scanf_s("%s", getInput, 10);
			printf(C_RESET);

			if (getInput[0] == '/')
			{
				if (!TryCommand(game, getInput))
				{
					printf("Unknown command \"" C_CYAN "%s" C_RESET "\". Type \"/help\".\n", getInput);
					goto inputLabel;
				}
			}
			else
			{
				Point src, dest;
				if (ReadMove(getInput, &src, &dest))
				{
					bool check, checkmate;
					Player enemy = EnemyOf(game->playerTurn);
					MoveData data = TryMove(game, XY(src), XY(dest));
					check = IsCheckForPlayer(game->mainBoard, enemy);
					if (data.attempt == Success)
					{
						CheckGameState(game);
						checkmate = (game->result == Checkmate);
						MoveText text;
						WriteMoveToBuffer(text, data, check, checkmate);
						printf("Move %d: %s\n", game->currentMove, text);
						strcpy(game->moves[game->currentMove], text);
						game->currentMove++;
						game->playerTurn = enemy;
						DrawGameBoard(game);
					}
					else goto inputLabel;
				}
				else
				{
					printf("Invalid move " C_CYAN "%s" C_RESET ". Try something like "  C_CYAN "e2e4" C_RESET ".\n", getInput);
					goto inputLabel;
				}
			}
		}
	}
	switch (game->result)
	{
	case Checkmate:
		printf(C_YELLOW "Checkmate! " C_RESET);
		break;
	case Stalemate:
		printf(C_YELLOW "Stalemate! " C_RESET);
		break;
	}
	switch (game->state)
	{
		case WhiteWon:
			printf("Congratulations! " C_UNDERLINE C_WHITE "%s" C_RESET " won the game.\n", PlayerToName(White));
			break;
		case BlackWon:
			printf("Congratulations! " C_UNDERLINE C_DGRAY "%s" C_RESET " won the game.\n", PlayerToName(Black));
			break;
		case Draw:
			printf("Game ended in draw.\n");
			break;
	}
	Command_Moves(game);
}

void DrawGameBoard(Game *game)
{
	Board *board = game->mainBoard;
	printf("\n");
	printf(C_BOARD_COORDS C_BOARD_BORDER "  ");
	for (int x = 0; x < BOARD_SIZE; x++)
	{
		printf("%c ", 'A' + x);
	}
	printf("  " C_RESET "\n");
	for (int y = BOARD_SIZE - 1; y >= 0; y--)
	{
		printf(C_BOARD_COORDS C_BOARD_BORDER "%d " C_RESET, y + 1);
		for (int x = 0; x < BOARD_SIZE; x++)
		{
			Piece piece = GetPiece(board, x, y);
			Player player = GetPlayer(board, x, y);

			if ((x + y) % 2) printf(C_BOARD_TILE1);
			else printf(C_BOARD_TILE2);
			if (player == White) printf(C_WHITE);
			else printf(C_BLACK);

			printf("%s ", PieceToChar(piece));
			printf(C_RESET);
		}
		printf(C_BOARD_BORDER C_BOARD_COORDS " %d" C_RESET "\n", y + 1);
	}
	printf(C_BOARD_COORDS C_BOARD_BORDER "  ");
	for (int x = 0; x < BOARD_SIZE; x++)
	{
		printf("%c ", 'A' + x);
	}
	printf("  " C_RESET "\n\n");
}

void Command_Help(Game *game)
{
	printf(C_YELLOW "List of available commands:\n" C_RESET);
	printf(C_CYAN "/help" C_RESET " - this command.\n");
	printf(C_CYAN "/moves" C_RESET " - prints all moves made during game.\n");
	printf(C_CYAN "/stats" C_RESET " - shows some info about game.\n");
	printf(C_CYAN "/redraw" C_RESET " - redraw game board.\n");
	printf(C_CYAN "/undo" C_RESET " - restore game state to previous move.\n");
}

void Command_Moves(Game *game)
{
	if (game->currentMove > 0)
	{
		printf(C_YELLOW "Moves: " C_RESET);
		for (int i = 0; i < game->currentMove; i++)
		{
			printf("%s ", game->moves[i]);
		}
		printf("\n");
	}
	else printf(C_YELLOW "No moves made yet.\n" C_RESET);
}

bool TryCommand(Game *game, const char *command)
{
	if (!strcmp(command, "/help"))
	{
		Command_Help(game);
		return true;
	}
	if (!strcmp(command, "/moves"))
	{
		Command_Moves(game);
		return true;
	}
	if (!strcmp(command, "/stats"))
	{
		return true;
	}
	if (!strcmp(command, "/redraw"))
	{
		return true;
	}
	if (!strcmp(command, "/undo"))
	{
		return true;
	}
	if (!strcmp(command, "/wcomp"))
	{
		return true;
	}
	if (!strcmp(command, "/bcomp"))
	{
		return true;
	}
	return false;
}

void CheckGameState(Game *game)
{
	if (game->state == WaitingForFirstMove && game->currentMove > 0) game->state = Playing;

	if (IsCheckmateForPlayer(game->mainBoard, White))
	{
		game->state = BlackWon;
		game->result = Checkmate;
	}
	if (IsCheckmateForPlayer(game->mainBoard, Black))
	{
		game->state = WhiteWon;
		game->result = Checkmate;
	}
	if (IsStalemateForPlayer(game->mainBoard, White) || IsStalemateForPlayer(game->mainBoard, Black))
	{
		game->state = Draw;
		game->result = Stalemate;
	}
}