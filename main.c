#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include "chessTypes.h"
#include "findMoves.h"
#include "moves.h"

#define THREAD_NUM 4

#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))

int jumps;

hashList_t *HashTable = NULL;

typedef struct searchPack_t
{
	uint8_t depth;
	position *myPos;
	uint8_t bestMoveNumber;
	uint8_t nominator;
	uint8_t denominator;
} searchPack_t;

pthread_mutex_t searchMutex;
pthread_mutex_t hashMutex;

// tables for position evaluation, to improve the piece arrangement according to better and worse places for each kind of piece
int pawnScores[64] = {8, 8, 8, 8, 8, 8, 8, 8,
					  6, 6, 6, 6, 6, 6, 6, 6,
					  3, 3, 3, 3, 3, 3, 3, 3,
					  2, 2, 2, 3, 3, 2, 2, 2,
					  1, 1, 2, 3, 3, 2, 1, 1,
					  1, 1, 0, 1, 1, 0, 1, 1,
					  0, 0, 0, 0, 0, 0, 0, 0,
					  0, 0, 0, 0, 0, 0, 0, 0};

int bishopScores[64] = {3, 3, 3, 3, 3, 3, 3, 3,
						3, 3, 3, 3, 3, 3, 3, 3,
						2, 3, 3, 3, 3, 3, 3, 3,
						2, 2, 3, 3, 3, 2, 2, 2,
						1, 1, 2, 2, 2, 2, 1, 1,
						1, 1, 1, 1, 1, 1, 1, 1,
						0, 2, 0, 0, 0, 0, 2, 0,
						0, 0, 0, 0, 0, 0, 0, 0};

int knightScores[64] = {2, 2, 2, 2, 2, 2, 2, 2,
						2, 2, 2, 2, 2, 2, 2, 2,
						0, 1, 2, 3, 3, 2, 1, 0,
						0, 1, 3, 4, 4, 3, 1, 0,
						0, 1, 2, 3, 3, 2, 1, 0,
						0, 1, 2, 2, 2, 2, 1, 0,
						0, 1, 1, 1, 1, 1, 1, 0,
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
					  0, 0, -2, -2, -2, -2, 0, 0,
					  0, 0, -2, -3, -3, -4, 0, 0,
					  0, 0, -2, -3, 1, 0, 5, 0};


void addHash(hashList_t *ptr, uint64_t val)
{
	hashList_t *current = ptr;
	while(current->next != NULL)
	{
		current = current->next;
	}

	current->next = (hashList_t*) malloc(sizeof(hashList_t));
	current->next->val = val;
	current->next->next = NULL;
}

int removeLastHash(hashList_t *ptr)
{
	int retVal = 0;
	if(ptr->next == NULL)
	{
		retVal = ptr->val;
		free(ptr);
		return retVal;
	}

	hashList_t *current = ptr;
	while(current->next->next != NULL)
	{
		current = current->next;
	}

	retVal = current->next->val;
	free(current->next);
	current->next = NULL;
	return retVal;
}

void print_list(hashList_t *ptr)
{
	hashList_t * current = ptr;

	while(current != NULL)
	{
		printf("%ld \n",current->val);
		current = current->next;
	}
}


void changeTurn(position *myPos)
{
	if (myPos->turn == WHITE_TO_MOVE)
		myPos->turn = BLACK_TO_MOVE;
	if (myPos->turn == BLACK_TO_MOVE)
		myPos->turn = WHITE_TO_MOVE;
}

uint64_t generateHash(position *myPos)
{
	uint64_t retVal[4];
	if (myPos == NULL)
	{
		perror("GenerateHash(), null pointer!");
		exit(1);
	}

	//SHA256((char *)myPos, sizeof(position), (unsigned char *)&retVal);
	return retVal[0];
}



void printPiece(int8_t piece)
{
	switch (piece)
	{
	case WHITE_PAWN:
		printf("WHITE_PAWN");
		break;

	case WHITE_ROOK:
		printf("WHITE_ROOK");
		break;

	case WHITE_KNIGHT:
		printf("WHITE_KNIGHT");
		break;

	case WHITE_BISHOP:
		printf("WHITE_BISHOP");
		break;

	case WHITE_KING:
		printf("WHITE_KING");
		break;

	case WHITE_QUEEN:
		printf("WHITE_QUEEN");
		break;

	case 0:
		printf("_");
		break;

	case BLACK_PAWN:
		printf("BLACK_PAWN");
		break;
	case BLACK_ROOK:
		printf("BLACK_ROOK");
break;

	case BLACK_KNIGHT:
		printf("BLACK_KNIGHT");
break;

	case BLACK_BISHOP:
		printf("BLACK_BISHOP");
break;

	case BLACK_KING:
		printf("BLACK_KING");
break;

	case BLACK_QUEEN:
		printf("BLACK_QUEEN");
break;

	default:
		printf("?");
	}
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
	printf("  __A___B___C___D___E___F___G___H__\n\n8 ");
	for (i = 0; i < 64; i++)
	{
		printf("| %c ", pieceToChar(myPos->myBoard[i]));

		if ((i + 1) % 8 == 0)
		{
			if ((64 - i) / 8)
				printf("| %d\n%d ", (64 - i) / 8 + 1, (64 - i) / 8);
			else
				printf("| %d \n  ", (64 - i) / 8 + 1);
		}
	}
	printf("_________________________________\n");
	printf("    A   B   C   D   E   F   G   H\n");
	if (myPos->turn == WHITE_TO_MOVE)
		printf("white to move! '\n");
	if (myPos->turn == BLACK_TO_MOVE)
		printf("black to move! '\n");
}

void setPositionFromFEN(position *myPos, char *fenStr)
{
	int i = 0, j = 0, k = 0;

	removeChar(fenStr, '/');
	myPos->blackCastleKing = 1;	 // 0 - means casle kingside is possible 1-impossible
	myPos->whiteCastleKing = 1;	 // 0 - means casle kingside is possible 1-impossible
	myPos->blackCastleQueen = 1; // 0 - means casle kingside is possible 1-impossible
	myPos->whiteCastleQueen = 1; // 0 - means casle kingside is possible 1-impossible

	memset(myPos->myBoard,0,sizeof(int8_t));
	for (i = 0; i < 67; i++)
	{
		if (fenStr[i] == ' ')
		{
			if (fenStr[i + 1] == 'w') // who's move is now ? w-white, b-black
				myPos->turn = WHITE_TO_MOVE;
			if (fenStr[i + 1] == 'b')
				myPos->turn = BLACK_TO_MOVE;

			for (k = i + 3; k < i + 7; k++) // check if castle king is possible
			{
				if (fenStr[k] == 'K')
					myPos->whiteCastleKing = 0;
				if (fenStr[k] == 'k')
					myPos->blackCastleKing = 0;
				if (fenStr[k] == 'Q')
					myPos->whiteCastleQueen = 0;
				if (fenStr[k] == 'q')
					myPos->blackCastleQueen = 0;
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

int Search2(int depth, position *myPos, int rec, uint8_t *moveNumber, int alpha, int beta) // alternative function with alpha-beta pruning, not tested yet
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

	findMoves(myPos, 1);

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
		unMakeMove(nextPos, 0);
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

int Search(int depth, position *myPos, int rec, uint8_t *moveNumber) // function for recursive searching of best move
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

	findMoves(myPos, 1);

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

		unMakeMove(nextPos, 0);
	}

	myPos->bestMoveNumber = nextPos->bestMoveNumber;
	myPos->bestMove = nextPos->bestMove;

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

int MultiSearch(int depth, position *myPos, int rec, uint8_t *moveNumber, int a, int b) // function for recursive searching of best move
{
	int i = 0;
	int evaluation = 0;
	int bestEvaluation = -9999999;
	static int j = 0;
	//	if (depth == 0)
	//	{
	//		evaluatePosition(myPos);
	//		return myPos->evaluation;
	//	}

	position *nextPos = malloc(sizeof(position)); // copy of position for searching the best move on it
	if(nextPos == NULL)
	{
		perror("failed to malloc \n");
		exit(1);

	}
	findMoves(myPos, 1);
	if(myPos->numberOfMoves == 0)
	{
		printf("CHECKMATE!!!!!!\n");
		exit(1);
	}
	*nextPos = *myPos;

	for (i = (a - 1) * (nextPos->numberOfMoves) / b; i <= a * (nextPos->numberOfMoves) / b - 1; i++)
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

		unMakeMove(nextPos, 0);
	}
		pthread_mutex_lock(&searchMutex);
		myPos->bestMoveNumber = nextPos->bestMoveNumber;
		myPos->bestMove = nextPos->bestMove;
		pthread_mutex_unlock(&searchMutex);

	free(nextPos);

	evaluation = -99999999;
	if (rec == 0)
	{
		for (i = (a - 1) * (nextPos->numberOfMoves) / b; i < a * (nextPos->numberOfMoves) / b - 1; i++)
		{
			if (myPos->moves[i].evaluation > evaluation)
			{
				evaluation = myPos->moves[i].evaluation;
				*moveNumber = i;
			}
		}
	}

	return bestEvaluation;
	// return NULL;
}

void *MultiStart(void *arg)
{
	searchPack_t *pack = (searchPack_t *)arg;
	MultiSearch(pack->depth, pack->myPos, 0, &pack->bestMoveNumber, pack->nominator, pack->denominator);
}

int main(int argc, char **argv)
{
	position myPos1;
	int ruch = 0;
	memset(&myPos1, 0, sizeof(position));
	int i = 0;
	move tmpMove, tmpMove2;

	int depth;
	int status1;
	int threads = 1;
	int bestEval = -99999;
	char buff[10];
	char startPos[100] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
	char testfen[100] = "r1bqkb1r/ppp1pppp/2np1n2/2PP1B2/8/8/PP2PPPP/RNBQK1NR w KQkq - 0 1";

	if (argc == 1)
	{
		printf("usage: %s -d [depth] -fen [FENSTR]\n", argv[0]);
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
			if (argv[4] != NULL)
				strcpy(startPos, argv[4]); // set the starting position from FEN_string argument
		}
	}

	if (argc > 5)
	{
		if (!strcmp("-t", argv[5]))
		{
			threads = atoi(argv[6]); // set the starting position from FEN_string argument
			printf("i set %d threads \n", threads);
		}
	}

	searchPack_t *pack = malloc(threads * sizeof(searchPack_t));


	pthread_t *searchThread = malloc(threads * sizeof(pthread_t));

	for (int i = 0; i < threads; i++)
	{
		pack[i].depth = depth;
		pack[i].nominator = i + 1;
		pack[i].denominator = threads;
	}

	memset(myPos1.myBoard, 0, 64 * sizeof(int8_t)); // finish here

	setPositionFromFEN(&myPos1, startPos);
	pthread_mutex_init(&searchMutex, NULL);
	pthread_mutex_init(&hashMutex, NULL);

	// isMovePossible(&tmpMove, &myPos1); //checks if the move is possible to make
	// printf("status = %d \n", status1);
//memset(myPos1.myBoard, 0, 64 * sizeof(int8_t)); // finish here
	uint8_t temp1, temp2;
	while (1)
	{
		printPosition(&myPos1);

		findMoves(&myPos1, 1);
		printMoves(&myPos1);

		//deleteMove(&myPos1,2);
		//printMoves(&myPos1);
	// makeMove(&myPos1,3);
	// printf("give the move number from above list: ");
	jeszcze:
	//	for (int i = 0; i < 63; i++)
	//	{
	//		printf("\n pole[%d](%d)=", i,myPos1.myBoard[i]);
	//		printPiece(myPos1.myBoard[i]);
	//	}
		// printf("\n hash %ld\n",generateHash(&myPos1));
		tmpMove = getMove(&status1);
		// printf("taki zczytalem: ");
		// printMove(&tmpMove);
		if (isMovePossible(&tmpMove, &myPos1) == 1)
			makeMoveFromMove(&myPos1, &tmpMove);
		else
		{
			printf("No such move possible\n Available list of moves:");
			printMoves(&myPos1);
			goto jeszcze;
		}

		// makeMove(&myPos1, ruch - 1);

		printPosition(&myPos1);
		memset(&myPos1.moves,0,sizeof(move)*MAX_MOVES);
		findMoves(&myPos1, 1);
		
			printMoves(&myPos1);

		for (int i = 0; i < threads; i++)
			pack[i].myPos = &myPos1;

		for (int i = 0; i < threads; i++)
			pthread_create(&searchThread[i], NULL, MultiStart, (void *)&pack[i]);

		for (int i = 0; i < threads; i++)
			pthread_join(searchThread[i], NULL);

		
		// Search(depth, &myPos1, 0, &myPos1.bestMoveNumber);
		// MultiSearch(depth, &myPos1, 0, &temp1,1,2);
		// MultiSearch(depth, &myPos1, 0, &temp2,2,2);

		printf("\n");

		for (int i = 0; i < threads; i++)
		{
			printMove(&myPos1.moves[pack[i].bestMoveNumber]);
			printf("[%d] ", myPos1.moves[i].evaluation);
		}
		printf("\n");
		//printMoves(&myPos1);
		printf("mozliwych ruchow jest: %d \n",myPos1.numberOfMoves);
		for (int i = 0; i < myPos1.numberOfMoves; i++) // searching from best_move candidates (each from diffrent thread) the best one
		{
				printf("kandydat na ruch [%d]:", myPos1.moves[i].evaluation);
				printMove(&myPos1.moves[i]);
				printf("\n");
			if (myPos1.moves[i].evaluation > bestEval)
			{
		
				bestEval = myPos1.moves[i].evaluation;
				// tmpMove2 = myPos1.moves[pack[i].bestMoveNumber];
				myPos1.bestMoveNumber = i; // replace the real one best Move namber
			}
		}
		printf("i moved to: ");
		printMove(&myPos1.moves[myPos1.bestMoveNumber]);
		// printf("i moved to: [%d]\n", myPos1.bestMoveNumber);

		makeMove(&myPos1, myPos1.bestMoveNumber);
		bestEval = -99999;
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

	pthread_mutex_destroy(&searchMutex);
	pthread_mutex_destroy(&hashMutex);
	return 0;
}
