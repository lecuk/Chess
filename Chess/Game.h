#ifndef GAME_H
#define GAME_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include "GameRules.h"

#define C_RESET			"\033[0m"
#define C_BOLD			"\033[1m"
#define C_FAINT			"\033[2m"
#define C_ITALIC		"\033[3m"
#define C_UNDERLINE		"\033[4m"

#define C_BLACK			"\033[38;5;232m"
#define C_RED			"\033[38;5;196m"
#define C_GREEN			"\033[38;5;46m"
#define C_YELLOW		"\033[38;5;226m"
#define C_BLUE			"\033[38;5;21m"
#define C_MAGENTA		"\033[38;5;201m"
#define C_CYAN			"\033[38;5;51m"
#define C_WHITE			"\033[38;5;255m"
#define C_LGRAY			"\033[38;5;250m"
#define C_DGRAY			"\033[38;5;240m"

#define C_BACK_BLACK	"\033[48;5;232m"
#define C_BACK_RED		"\033[48;5;196m"
#define C_BACK_GREEN	"\033[48;5;46m"
#define C_BACK_YELLOW	"\033[48;5;226m"
#define C_BACK_BLUE		"\033[48;5;21m"
#define C_BACK_MAGENTA	"\033[48;5;201m"
#define C_BACK_CYAN		"\033[48;5;51m"
#define C_BACK_WHITE	"\033[48;5;231m"
#define C_BACK_LGRAY	"\033[48;5;250m"
#define C_BACK_DGRAY	"\033[48;5;238m"

#define C_BOARD_TILE1	"\033[48;5;172m"
#define C_BOARD_TILE2	"\033[48;5;196m"
#define C_BOARD_BORDER	"\033[48;5;124m"
#define C_BOARD_COORDS	"\033[38;5;220m"

typedef char MoveText[8];

typedef enum GameState
{
	WaitingForFirstMove, Playing, WhiteWon, BlackWon, Draw, Stopped
} GameState;

typedef enum GameResult
{
	Checkmate, Stalemate, NotEnoughMaterial, AgreedToDraw, Resign, Timeout, UnknownResult
} GameResult;

typedef struct Game
{
	Board *mainBoard;
	int currentMove;
	Board *positions[MAX_MOVES];
	MoveText moves[MAX_MOVES];
	Player playerTurn;
	GameState state;
	GameResult result;
	bool whiteComputerActive, blackComputerActive;
} Game;

Point ReadChessNotationPosition(char *position);
char *PlayerToName(Player player);
void MoveToText(MoveText text, DOUBLE_COORDS_PARAMS);
Point ReadChessNotationPosition(char *position);
bool ReadMove(char *getInput, Point *src, Point *dest);
MoveData TryMove(Game *game, DOUBLE_COORDS_PARAMS);
char *PieceToName(Piece piece);
char *PieceToChar(Piece piece);
Game *StartNewGame();
Game *StartGameFromPosition(const Position position);
void PlayGame(Game *game);
void CheckGameState(Game *game);
bool TryCommand(Game *game, const char *command);
void DrawGameBoard(Game *game);
void WriteMoveToBuffer(MoveText buffer, MoveData data, bool check, bool checkmate);
PromotionSelection GetPromotionInput();

void Command_Help(Game *game);
void Command_Moves(Game *game);

#endif