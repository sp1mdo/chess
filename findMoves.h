#ifndef FINDMOVES_H
#define FINDMOVES_H

#define WHITE_TO_MOVE 1
#define BLACK_TO_MOVE -1

void findMoves(position* myPos,int check);
int ifTakeKing(position *myPos);
int deleteMove(position *myPos, int moveNumber);
void deleteCheck(position *myPos);
#endif