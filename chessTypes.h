#ifndef CHESSTYPES_H
#define CHESSTYPES_H

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
	int from;
	int to;
	int evaluation;
	int promotion; //kind of piece to promote
} move;



typedef struct position
{
	int bestMoveNumber;
	char fenStr[100];
	int myBoard[64];
	move moves[100];
	move lastMove;
	move bestMove;
	int lastTakenPiece;
	int numberOfMoves;
	int turn;
	int blackCastleKing;
	int blackCastleQueen;
	int whiteCastleKing;
	int whiteCastleQueen;
	int evaluation;
	int exitDepth;
} position;


#endif