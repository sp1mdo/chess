#include "moves.h"
#include "chessTypes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void makeMove(position *myPos, int moveNo)
{
	// printMove(&myPos->moves[moveNo]);

	if (myPos->numberOfMoves > 0)
	{
		myPos->lastMove = myPos->moves[moveNo];

		myPos->lastTakenPiece = myPos->myBoard[myPos->moves[moveNo].to];
		myPos->myBoard[myPos->moves[moveNo].to] = myPos->myBoard[myPos->moves[moveNo].from];
		myPos->myBoard[myPos->moves[moveNo].from] = 0;

        //biala roszada king side
        if(myPos->moves[moveNo].from == 60 && myPos->moves[moveNo].to == 62 && myPos->myBoard[62] == WHITE_KING)
        {
            myPos->myBoard[61] = myPos->myBoard[63]; 
            myPos->myBoard[63] = 0;
            myPos->whiteCastleKing = 1;
            myPos->whiteCastleQueen = 1;
        }
        //biala roszada queen side
        if(myPos->moves[moveNo].from == 60 && myPos->moves[moveNo].to == 58 && myPos->myBoard[58] == WHITE_KING)
        {
            myPos->myBoard[59] = myPos->myBoard[56]; 
            myPos->myBoard[56] = 0;
            myPos->whiteCastleKing = 1;
            myPos->whiteCastleQueen = 1;
        }

        //czarna roszada king side
        if(myPos->moves[moveNo].from == 4 && myPos->moves[moveNo].to == 6 && myPos->myBoard[6] == BLACK_KING)
        {
            myPos->myBoard[61] = myPos->myBoard[63]; 
            myPos->myBoard[63] = 0;
            myPos->blackCastleKing = 1;
            myPos->blackCastleQueen = 1;
        }
        //czarna roszada queen side
        if(myPos->moves[moveNo].from == 4 && myPos->moves[moveNo].to == 2 && myPos->myBoard[2] == BLACK_KING)
        {
            myPos->myBoard[3] = myPos->myBoard[5]; 
            myPos->myBoard[5] = 0;
            myPos->blackCastleKing = 1;
            myPos->blackCastleQueen = 1;
        }


	}


	if (myPos->turn == WHITE_TO_MOVE && myPos->moves[moveNo].to < 8)
	{
		myPos->myBoard[myPos->moves[moveNo].to] = WHITE_QUEEN;
		myPos->lastMove.promotion = WHITE_QUEEN;
	}
	else
		myPos->lastMove.promotion = 0;
	if (myPos->turn == BLACK_TO_MOVE && myPos->moves[moveNo].to > 55)
	{
		myPos->myBoard[myPos->moves[moveNo].to] = BLACK_QUEEN;
		myPos->lastMove.promotion = BLACK_QUEEN;
	}
	else
		myPos->lastMove.promotion = 0;

	myPos->turn = myPos->turn * (-1);
	clearAllMoves(myPos);
}

void makeMoveFromMove(position *myPos, move *myMove)
{
	for (int i = 0; i < myPos->numberOfMoves; i++)
	{
		if (myMove->from == myPos->moves[i].from && myMove->to == myPos->moves[i].to)
		{
			makeMove(myPos, i);
		}
	}
}


int clearAllMoves(position *myPos)
{
	int i = 0;

	for (i = 0; i < 100; i++)
	{
		myPos->moves[i].to = 0;
		myPos->moves[i].from = 0;
	}
	myPos->numberOfMoves = 0;

	return 0;
}


void printMoves(position *myPos)
{
	int i = 0;

	char from[3] = {0}, to[3] = {0};

	for (i = 0; i < myPos->numberOfMoves; i++)
	{
		coordToStr(from, myPos->moves[i].from);
		coordToStr(to, myPos->moves[i].to);
		// printf("%d from: %d, to: %d\n",i,myPos->moves[i].from, myPos->moves[i].to);
		printf("%d. %s-%s  (%d)\n", i + 1, from, to, myPos->moves[i].evaluation);
	}
}

void printMove(move *myMove)
{
	char from[3] = {0}, to[3] = {0};

	coordToStr(from, myMove->from);
	coordToStr(to, myMove->to);
	printf("(%s-%s) ", from, to);
}

void unMakeMove(position *myPos)
{
	myPos->myBoard[myPos->lastMove.from] = myPos->myBoard[myPos->lastMove.to];
	myPos->myBoard[myPos->lastMove.to] = myPos->lastTakenPiece;
	// COFAnie ruchu gdy ostatniim ruchem byla prpomocja pionka, aby nie zostal
	if (myPos->lastMove.promotion && myPos->turn == BLACK_TO_MOVE)
	{
		myPos->myBoard[myPos->lastMove.from] = WHITE_PAWN;
	}
	if (myPos->lastMove.promotion && myPos->turn == WHITE_TO_MOVE)
	{
		myPos->myBoard[myPos->lastMove.from] = BLACK_PAWN;
	}

	myPos->turn = myPos->turn * (-1); // CIEKAWE CZY TO W OGOLE DZIALA...
	findMoves(myPos);
}

int isMovePossible(move *myMove, position *myPos)
{
	int retVal = -1; //-1 means no such move possible, 1 means move is possible
					 //	printMove(myMove);
	//	printf("=-=-");
	for (int i = 0; i < myPos->numberOfMoves; i++)
	{

		// printMove(&(myPos->moves[i]));
		if (myMove->from == myPos->moves[i].from && myMove->to == myPos->moves[i].to)
		{
			return 1;
		}
	}
	return retVal;
}

move getMove(int *status)
{
	*status = -1;
	char buff[7];
	move retVal;
	while (*status == -1)
	{
		begin:
		printf("Enter the move: ");
		scanf("%s", buff); // flush ?
		//printf("wzielem [%s]\n", buff);
		if(!strcmp(buff,"quit")) exit(0);
		if(!strcmp(buff,"?"))
		{
			puts("possible commands:");
			puts("e2-e4\t- example format of command given to make move from e2 to e4");
			puts("quit\t - exit the program");
			puts("?\t - display the command list");
		goto begin;
		};

		retVal = str2move(buff, status);
		return retVal;
	}
}

void coordToStr(char *dst, int coord)
{
	dst[0] = 'a' + (coord % 8);
	dst[1] = '0' + (63 - coord) / 8 + 1;
}

move str2move(char *txt, int *status)
{
	*status = -1; //-1 wrong, 1=ok
	move retVal;
	int fromLetter, toLetter;
	int fromNumber, toNumber;
	fromLetter = txt[0];
	fromNumber = txt[1] - 48;
	toLetter = txt[3];
	toNumber = txt[4] - 48;

	if (fromLetter >= 'a' && fromLetter <= 'h' && toLetter >= 'a' && toLetter <= 'h' && fromNumber >= 1 && fromNumber <= 8 && toNumber >= 1 && toNumber <= 8)
	{
		retVal.from = fromLetter - 97 + 8 * (8 - fromNumber);
		retVal.to = toLetter - 97 + 8 * (8 - toNumber);
		*status = 1;
		return retVal;
	}
	else
	{
		*status = -1; //wrong format of given move
		return retVal;
	}
}
