
#ifndef CHESSTYPES_H
#define CHESSTYPES_H

#include <inttypes.h>

#define MAX_MOVES 100

#define PAWN_VALUE 10
#define BISHOP_VALUE 30
#define KNIGHT_VALUE 30
#define QUEEN_VALUE 90
#define KING_VALUE 900
#define ROOK_VALUE 50

#define WHITE_KING 1
#define WHITE_PAWN 2
#define WHITE_QUEEN 3
#define WHITE_ROOK 4
#define WHITE_BISHOP 5
#define WHITE_KNIGHT 6

#define BLACK_KING -1
#define BLACK_PAWN -2
#define BLACK_QUEEN -3
#define BLACK_ROOK -4
#define BLACK_BISHOP -5
#define BLACK_KNIGHT -6

#define WHITE_TO_MOVE 1
#define BLACK_TO_MOVE -1

typedef struct move
{
	uint8_t from;
	uint8_t to;
	short evaluation;
	uint8_t promotion; //kind of piece to promote
} move;



typedef struct position
{
	uint8_t bestMoveNumber;
	//char fenStr[100];
	int8_t myBoard[64];
	int8_t lastTakenPiece;
	uint8_t numberOfMoves;
	int8_t turn;
	uint8_t blackCastleKing;
	uint8_t blackCastleQueen;
	uint8_t whiteCastleKing;
	uint8_t whiteCastleQueen;
	move moves[MAX_MOVES];
	move lastMove;
	move bestMove;
	short evaluation;
	short exitDepth;
} position;


#endif
