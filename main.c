#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "chessTypes.h"
#include "findMoves.h"
#include "moves.h"

#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))

int jumps;

// tables for position evaluation, to improve the piece arrangement according to better and worse places for each kind of piece
int pawnScores[64] = {8, 8, 8, 8, 8, 8, 8, 8,
					  6, 6, 6, 6, 6, 6, 6, 6,
					  2, 3, 4, 4, 4, 4, 3, 3,
					  2, 2, 3, 3, 3, 2, 2, 2,
					  1, 1, 2, 2, 2, 2, 1, 1,
					  1, 1, 1, 1, 1, 1, 1, 1,
					  0, 0, 0, 0, 0, 0, 0, 0,
					  0, 0, 0, 0, 0, 0, 0, 0};

int bishopScores[64] = {5, 5, 5, 5, 5, 5, 5, 5,
						5, 5, 5, 5, 5, 5, 5, 5,
						2, 3, 4, 4, 4, 4, 3, 3,
						2, 2, 3, 3, 3, 2, 2, 2,
						1, 1, 2, 2, 2, 2, 1, 1,
						1, 1, 1, 1, 1, 1, 1, 1,
						0, 0, 0, 0, 0, 0, 0, 0,
						0, 0, 0, 0, 0, 0, 0, 0};

int knightScores[64] = {3, 2, 2, 2, 2, 2, 2, 3,
						2, 2, 2, 2, 2, 2, 2, 2,
						1, 2, 3, 3, 3, 3, 2, 1,
						1, 2, 3, 4, 4, 3, 2, 1,
						1, 2, 3, 4, 4, 3, 2, 1,
						1, 2, 3, 4, 4, 3, 2, 1,
						1, 2, 2, 2, 2, 2, 2, 1,
						0, 0, 0, 0, 0, 0, 0, 0};

int rookScores[64] = {9, 9, 9, 9, 9, 9, 9, 9,
					  8, 8, 8, 8, 8, 8, 8, 8,
					  7, 7, 7, 7, 7, 7, 7, 7,
					  6, 6, 6, 6, 6, 6, 6, 6,
					  4, 5, 5, 5, 5, 5, 5, 4,
					  3, 4, 4, 4, 4, 4, 4, 3,
					  1, 2, 2, 3, 3, 2, 2, 1,
					  0, 0, 4, 2, 2, 4, 0, 0};

int queenScores[64] = {9, 9, 9, 9, 9, 9, 9, 9,
					   8, 8, 8, 8, 8, 8, 8, 8,
					   7, 7, 7, 7, 7, 7, 7, 7,
					   6, 6, 6, 6, 6, 6, 6, 6,
					   4, 5, 5, 5, 5, 5, 5, 4,
					   3, 4, 4, 4, 4, 4, 4, 3,
					   1, 2, 2, 3, 3, 2, 2, 1,
					   0, 0, 4, 2, 2, 4, 0, 0};

int kingScores[64] = {0, 0, 0, 0, 0, 0, 0, 0,
					  0, 0, 0, 0, 0, 0, 0, 0,
					  0, 0, 0, 0, 0, 0, 0, 0,
					  0, 0, 0, 0, 0, 0, 0, 0,
					  0, 0, 0, 0, 0, 0, 0, 0,
					  0, 0, 0, 0, 0, 0, 0, 0,
					  0, 0, 0, 0, 0, 0, 0, 0,
					  0, 0, 0, 0, 1, 0, 5, 0};

void changeTurn(position *myPos)
{
	if (myPos->turn == WHITE_TO_MOVE)
		myPos->turn = BLACK_TO_MOVE;
	if (myPos->turn == BLACK_TO_MOVE)
		myPos->turn = WHITE_TO_MOVE;
}

int charToPiece(char myChar)
{
	int retval;

	switch (myChar)
	{

	case 'K':
		retval = WHITE_KING;
		break;
	case 'P':
		retval = WHITE_PAWN;
		break;
	case 'Q':
		retval = WHITE_QUEEN;
		break;
	case 'R':
		retval = WHITE_ROOK;
		break;
	case 'B':
		retval = WHITE_BISHOP;
		break;
	case 'N':
		retval = WHITE_KNIGHT;
		break;
	case 'k':
		retval = BLACK_KING;
		break;
	case 'p':
		retval = BLACK_PAWN;
		break;
	case 'q':
		retval = BLACK_QUEEN;
		break;
	case 'r':
		retval = BLACK_ROOK;
		break;
	case 'b':
		retval = BLACK_BISHOP;
		break;
	case 'n':
		retval = BLACK_KNIGHT;
		break;

	default:

		retval = 0;
	}
	return retval;
}

void removeChar(char *str, char garbage) // usefull tool for removing some characters in FEN string
{

	char *src, *dst;
	for (src = dst = str; *src != '\0'; src++)
	{
		*dst = *src;
		if (*dst != garbage)
			dst++;
	}
	*dst = '\0';
}

char pieceToChar(int piece) // convert from english piece nomenclature to macrodefinition of each kind of piece
{
	char retval = 0;

	switch (piece)
	{

	case WHITE_KING:
		retval = 'K';
		break;
	case WHITE_PAWN:
		retval = 'P';
		break;
	case WHITE_QUEEN:
		retval = 'Q';
		break;
	case WHITE_ROOK:
		retval = 'R';
		break;
	case WHITE_BISHOP:
		retval = 'B';
		break;
	case WHITE_KNIGHT:
		retval = 'N';
		break;

	case BLACK_KING:
		retval = 'k';
		break;
	case BLACK_PAWN:
		retval = 'p';
		break;
	case BLACK_QUEEN:
		retval = 'q';
		break;
	case BLACK_ROOK:
		retval = 'r';
		break;
	case BLACK_BISHOP:
		retval = 'b';
		break;
	case BLACK_KNIGHT:
		retval = 'n';
		break;
	case 0:
		retval = ' ';
	default:
		retval = ' ';
	}

	return retval;
}

void evaluatePosition(position *myPos) // function evaluates the position based on posessed material and positoin nuances.
{
	int eval = 0, i = 0;

	for (i = 0; i < 64; i++)
	{
		if (myPos->myBoard[i] == WHITE_PAWN)
		{
			eval = eval + PAWN_VALUE;
			eval = eval + pawnScores[i];
		}

		if (myPos->myBoard[i] == WHITE_KNIGHT)
		{
			eval = eval + KNIGHT_VALUE;
			eval = eval + knightScores[i];
		}

		if (myPos->myBoard[i] == WHITE_ROOK)
		{
			eval = eval + ROOK_VALUE;
			eval = eval + rookScores[i];
		}

		if (myPos->myBoard[i] == WHITE_KING)
		{
			eval = eval + KING_VALUE;
			eval = eval + kingScores[i];
		}

		if (myPos->myBoard[i] == WHITE_QUEEN)
		{
			eval = eval + QUEEN_VALUE;
			eval = eval + queenScores[i];
		}

		if (myPos->myBoard[i] == BLACK_PAWN)
		{
			eval = eval - PAWN_VALUE;
			eval = eval - pawnScores[64 - i];
		}

		if (myPos->myBoard[i] == BLACK_KNIGHT)
		{
			eval = eval - KNIGHT_VALUE;
			eval = eval - knightScores[64 - i];
		}

		if (myPos->myBoard[i] == BLACK_ROOK)
		{
			eval = eval - ROOK_VALUE;
			eval = eval - rookScores[64 - i];
		}

		if (myPos->myBoard[i] == BLACK_KING)
		{
			eval = eval - KING_VALUE;
			eval = eval - kingScores[64 - i];
		}

		if (myPos->myBoard[i] == BLACK_QUEEN)
		{
			eval = eval - QUEEN_VALUE;
			eval = eval - queenScores[64 - i];
		}
	}

	if (myPos->turn == BLACK_TO_MOVE)
		eval = -eval; // negation of evaluation from black point of view
	myPos->evaluation = eval;
}

void printPosition(position *myPos) // function that prints the board layout on the console
{
	int i;
	printf("__A___B___C___D___E___F___G___H__\n\n");
	for (i = 0; i < 64; i++)
	{
		printf("| %c ", pieceToChar(myPos->myBoard[i]));

		if ((i + 1) % 8 == 0)
			printf("| \n");
	}
	printf("_________________________________\n");
	printf("  A   B   C   D   E   F   G   H\n");
	if (myPos->turn == WHITE_TO_MOVE)
		printf("white to move! '\n");
	if (myPos->turn == BLACK_TO_MOVE)
		printf("black to move! '\n");
}

void setPositionFromFEN(position *myPos, char *fenStr)
{
	int i = 0, j = 0, k=0;

	removeChar(fenStr, '/');
	myPos->blackCastleKing = 1;	 // 0 - means casle kingside is possible 1-impossible
	myPos->whiteCastleKing = 1;	 // 0 - means casle kingside is possible 1-impossible
	myPos->blackCastleQueen = 1; // 0 - means casle kingside is possible 1-impossible
	myPos->whiteCastleQueen = 1; // 0 - means casle kingside is possible 1-impossible

	for (i = 0; i < 67; i++)
	{
		if (fenStr[i] == ' ')
		{
			if (fenStr[i + 1] == 'w') // who's move is now ? w-white, b-black
				myPos->turn = WHITE_TO_MOVE;
			if (fenStr[i + 1] == 'b')
				myPos->turn = BLACK_TO_MOVE;
			
			for(k=i+3 ; k<i+7 ; k++) //check if castle king is possible
			{ 
				if (fenStr[k] == 'K') myPos->whiteCastleKing = 0;
				if (fenStr[k] == 'k') myPos->blackCastleKing = 0;
				if (fenStr[k] == 'Q') myPos->whiteCastleQueen = 0;
				if (fenStr[k] == 'q') myPos->blackCastleQueen = 0;
			
			}
			break;
		}

		// printf("%d = %d = %c \n",j,charToPiece(fenStr[i]),fenStr[i]);
		// printf("i=%d ",i);
		if (isalpha(fenStr[i]))
		{
			myPos->myBoard[j] = charToPiece(fenStr[i]);
			// printf("literka\n");
		}
		if (isdigit(fenStr[i]))
		{
			j = j + fenStr[i] - '0' - 1;
			//	printf("cyferka\n");
		}
		//	else if(fenStr[i] == '/') i++;
		j++;
	}
}

int coordCalc(int file, int rank) // function converts combination of file(1-8) and rank(a-h) to number of cell (0-63)
{
	return file * 8 + rank;
}

int Search2(int depth, position *myPos, int rec, int *moveNumber, int alpha, int beta) // alternative function with alpha-beta pruning, not tested yet
{
	int i = 0;
	int evaluation = 0;
	int bestEvaluation = 0;
	int j = 0;
	if (depth == 0)
	{
		evaluatePosition(myPos);
		return myPos->evaluation;
	}

	position *nextPos = calloc(1, sizeof(position));

	findMoves(myPos);

	*nextPos = *myPos;

	bestEvaluation = -9999999;
	for (i = 0; i < nextPos->numberOfMoves; i++)
	{
		makeMove(nextPos, i);
		evaluation = -Search2(depth - 1, nextPos, 1, NULL, -alpha, -beta);
		if (rec == 0)
			myPos->moves[i].evaluation = evaluation;
		if (evaluation >= beta)
		{
			if (rec == 0)
				*moveNumber = i;
			return beta;
		}
		alpha = max(alpha, evaluation);
		//	if(beta <= alpha) break;

		//	bestEvaluation = max(bestEvaluation,evaluation);
		unMakeMove(nextPos);
	}

	myPos->bestMoveNumber = nextPos->bestMoveNumber;
	myPos->bestMove = nextPos->bestMove;
	// myPos->exitDepth = depth;
	//*myPos = *nextPos;

	free(nextPos);

	evaluation = -99999999;
	if (rec == 0)
	{
		for (i = 0; i < myPos->numberOfMoves; i++)
		{
			if (myPos->moves[i].evaluation > evaluation)
			{
				evaluation = myPos->moves[i].evaluation;
				*moveNumber = i;
			}
		}
	}

	return alpha;
}

int Search(int depth, position *myPos, int rec, int *moveNumber) // function for recursive searching of best move
{
	int i = 0;
	int evaluation = 0;
	int bestEvaluation = 0;
	static int j = 0;
	if (depth == 0)
	{
		evaluatePosition(myPos);
		return myPos->evaluation;
	}
	j++;
	if (j % 101 == 0)
		printf("\r%d", j); // control output to see the progress and number of evaluated positions

	position *nextPos = calloc(1, sizeof(position)); // copy of position for searching the best move on it

	findMoves(myPos);

	*nextPos = *myPos;

	bestEvaluation = -9999999;
	for (i = 0; i < nextPos->numberOfMoves; i++)
	{
		makeMove(nextPos, i);
		evaluation = -Search(depth - 1, nextPos, 1, NULL); // best for us is worst for the opponent
		if (rec == 0)
			myPos->moves[i].evaluation = evaluation;
		if (evaluation > bestEvaluation)
		{
			bestEvaluation = evaluation;
			if (rec == 0)
				*moveNumber = i;
		}
		//	alpha = max(alpha,evaluation);
		//	if(beta <= alpha) break;

		//	bestEvaluation = max(bestEvaluation,evaluation);
		unMakeMove(nextPos);
	}

	myPos->bestMoveNumber = nextPos->bestMoveNumber;
	myPos->bestMove = nextPos->bestMove;
	// myPos->exitDepth = depth;
	//*myPos = *nextPos;

	free(nextPos);

	evaluation = -99999999;
	if (rec == 0)
	{
		for (i = 0; i < myPos->numberOfMoves; i++)
		{
			if (myPos->moves[i].evaluation > evaluation)
			{
				evaluation = myPos->moves[i].evaluation;
				*moveNumber = i;
			}
		}
	}

	return bestEvaluation;
}

int main(int argc, char **argv)
{
	position myPos1;
	int ruch = 0;
	memset(&myPos1, 0, sizeof(position));
	int i = 0;
	move tmpMove;
	int depth;
	int status1;
	char buff[10];
	char startPos[100] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
	char testfen[100] = "r1bqkb1r/ppp1pppp/2np1n2/2PP1B2/8/8/PP2PPPP/RNBQK1NR w KQkq - 0 1";

	if(argc == 1)
		{
		printf("usage: %s -d [depth] -fen [FENSTR]\n",argv[0]);
		return 1;
		}

	if (argv[1] == NULL || argv[2] == NULL) // checking if the parameters for depth is given
	{
		printf("no depth parameter, set depth=4 by default\n");
		depth = 4;
	}
	else
	{
		if (!strcmp("-d", argv[1]))
		{
			depth = atoi(argv[2]);
			printf("i set depth to %d\n", depth);
		}
	}
	
	if (argc > 3)
	{
		if (!strcmp("-fen", argv[3]))
		{
			printf("FENIK [%s]\n", argv[4]);
		}
	}

	if(argv[4] != NULL) strcpy(startPos,argv[4]); //set the starting position from FEN_string argument


	setPositionFromFEN(&myPos1, startPos);

	printf("\n %d %d %d %d = roszady \n", myPos1.whiteCastleKing, myPos1.whiteCastleQueen, myPos1.blackCastleKing, myPos1.blackCastleQueen);

	// isMovePossible(&tmpMove, &myPos1); //checks if the move is possible to make
	// printf("status = %d \n", status1);

	while (1)
	{
		printPosition(&myPos1);

		findMoves(&myPos1);
		// printMoves(&myPos1);
	// makeMove(&myPos1,3);
	// printf("give the move number from above list: ");
	jeszcze:
		tmpMove = getMove(&status1);
		// printf("taki zczytalem: ");
		// printMove(&tmpMove);
		if (isMovePossible(&tmpMove, &myPos1) == 1)
			makeMoveFromMove(&myPos1, &tmpMove);
		else
		{
			printf("nima tak !!  \n");
			goto jeszcze;
		}

		// makeMove(&myPos1, ruch - 1);

		printPosition(&myPos1);
		findMoves(&myPos1);
		//	printMoves(&myPos1);
		Search(depth, &myPos1, 0, &myPos1.bestMoveNumber);
		printf("\n");
		printf("i moved to: ");

		printMove(&myPos1.moves[myPos1.bestMoveNumber]);
		// printf("i moved to: [%d]\n", myPos1.bestMoveNumber);

		makeMove(&myPos1, myPos1.bestMoveNumber);
		printf("\n");
	}

	printf("i moved to: ");

	evaluatePosition(&myPos1);
	printf("evaluation = %d \n", Search(depth, &myPos1, 0, &myPos1.bestMoveNumber));
	printf("%d---\n", myPos1.bestMoveNumber);
	printMove(&myPos1.moves[myPos1.bestMoveNumber]);
	printf("\n");
	printMoves(&myPos1);

	printf("best move is %d\n", myPos1.bestMoveNumber + 1);
	printf("exitdepth=%d\n", myPos1.exitDepth);

	return 0;
}
