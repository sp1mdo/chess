#ifndef MOVES_H
#define MOVES_H
#include "chessTypes.h"
#include "findMoves.h"

void makeMove(position *myPos, int moveNo);
void makeMoveFromMove(position *myPos, move *myMove);
int clearAllMoves(position *myPos);
void printMoves(position *myPos);
void printMove(move *myMove);
void unMakeMove(position *myPos);
int isMovePossible(move *myMove, position *myPos);
move getMove(int *status);
void coordToStr(char *dst, int coord);
move str2move(char *txt, int *status);


#endif