#include "chessTypes.h"
#include "findMoves.h"

int isRightBound(int coord) //function checks if it's H column
{
	if ((coord + 1) % 8 == 0)
		return 1;
	else return 0;
}

int isLeftBound(int coord)  //function checks if it's A column
{
	if ((coord) % 8 == 0)
		return 1;
	else return 0;
}


int isDownBound(int coord) //functoin checks if it's 1-st row
{
	if (coord >= 56 && coord <= 63) return 1;
	else return 0;

}

int isUpBound(int coord) //function checks if it's 8th row
{

	if (coord >= 0 && coord <= 7) return 1;
	else return 0;

}


int ifPieceTake(position* myPos, int to) //function checks if target position has a piece of opposite color (to take!)
{
	if ((myPos->myBoard[to] < 0 && myPos->turn == WHITE_TO_MOVE) || (myPos->myBoard[to] > 0 && myPos->turn == BLACK_TO_MOVE))
		return 1;
	else
		return 0;
}


int rankFromCoord(int coord) //convert rank number (A-H) from coord (0-63)
{
	return (63 - coord) / 8 + 1;
}

int fileFromCoord(int coord) //convert file number (1-8) from coord (0-63)
{
	return (coord % 8) + 1;
}

void findBishopMoves(position* myPos, int i) //funding all bishops moves
{
	int j = 0;
	if (isUpBound(i) == 0 && isLeftBound(i) == 0) //upper left
	{

		while (isUpBound(i + j) == 0 && isLeftBound(i + j) == 0)//if we are not out the upper-left bound
		{
			j = j - 9;
			if ((myPos->myBoard[i + j] <= 0 && myPos->turn == WHITE_TO_MOVE) || (myPos->myBoard[i + j] >= 0 && myPos->turn == BLACK_TO_MOVE))
			{
				myPos->moves[myPos->numberOfMoves].from = i;
				myPos->moves[myPos->numberOfMoves].to = i + j;
				myPos->numberOfMoves++;
				if (ifPieceTake(myPos, i + j)) break;
			}
			if ((myPos->myBoard[i + j] > 0 && myPos->turn == WHITE_TO_MOVE) || (myPos->myBoard[i + j] < 0 && myPos->turn == BLACK_TO_MOVE)) break; //stop iteration if next field has the same color piece

		}
	}

	j = 0;
	if (isDownBound(i) == 0 && isRightBound(i) == 0) //lower right
	{

		while (isDownBound(i + j) == 0 && isRightBound(i + j) == 0)//if we are outside lower-right bound
		{
			j = j + 9;
			if ((myPos->myBoard[i + j] <= 0 && myPos->turn == WHITE_TO_MOVE) || (myPos->myBoard[i + j] >= 0 && myPos->turn == BLACK_TO_MOVE))
			{
				myPos->moves[myPos->numberOfMoves].from = i;
				myPos->moves[myPos->numberOfMoves].to = i + j;
				myPos->numberOfMoves++;
				if (ifPieceTake(myPos, i + j)) break;
			}
			if ((myPos->myBoard[i + j] > 0 && myPos->turn == WHITE_TO_MOVE) || (myPos->myBoard[i + j] < 0 && myPos->turn == BLACK_TO_MOVE)) break; //stop iteration if next field has the same color piece

		}
	}

	j = 0;
	if (isDownBound(i) == 0 && isLeftBound(i) == 0) //lower left
	{

		while (isDownBound(i + j) == 0 && isLeftBound(i + j) == 0)//if we are outside the lower left bound
		{
			j = j + 7;
			if ((myPos->myBoard[i + j] <= 0 && myPos->turn == WHITE_TO_MOVE) || (myPos->myBoard[i + j] >= 0 && myPos->turn == BLACK_TO_MOVE))
			{
				myPos->moves[myPos->numberOfMoves].from = i;
				myPos->moves[myPos->numberOfMoves].to = i + j;
				myPos->numberOfMoves++;
				if (ifPieceTake(myPos, i + j)) break;
			}
			if ((myPos->myBoard[i + j] > 0 && myPos->turn == WHITE_TO_MOVE) || (myPos->myBoard[i + j] < 0 && myPos->turn == BLACK_TO_MOVE)) break; //stop iteration if next field has the same color piece

		}
	}

	j = 0;
	if (isUpBound(i) == 0 && isRightBound(i) == 0) //uppwer-right
	{
		while (isUpBound(i + j) == 0 && isRightBound(i + j) == 0)//if we are outside the upper-right bound
		{
			j = j - 7;
			if ((myPos->myBoard[i + j] <= 0 && myPos->turn == WHITE_TO_MOVE) || (myPos->myBoard[i + j] >= 0 && myPos->turn == BLACK_TO_MOVE))
			{
				myPos->moves[myPos->numberOfMoves].from = i;
				myPos->moves[myPos->numberOfMoves].to = i + j;
				myPos->numberOfMoves++;
				if (ifPieceTake(myPos, i + j)) break;
			}
			if ((myPos->myBoard[i + j] > 0 && myPos->turn == WHITE_TO_MOVE) || (myPos->myBoard[i + j] < 0 && myPos->turn == BLACK_TO_MOVE)) break; //stop iteration if next field has the same color piece

		}
	}

	j = 0;
}

void findKingMoves(position* myPos, int i)
{
	//down
	if (rankFromCoord(i) > 1) {
		if ((myPos->myBoard[i + 8] <= 0 && myPos->turn == WHITE_TO_MOVE) || (myPos->myBoard[i + 8] >= 0 && myPos->turn == BLACK_TO_MOVE))
		{
			myPos->moves[myPos->numberOfMoves].from = i;
			myPos->moves[myPos->numberOfMoves].to = i + 8;
			myPos->numberOfMoves++;
		}
	}
	//up
	if (rankFromCoord(i) < 8) {
		if ((myPos->myBoard[i - 8] <= 0 && myPos->turn == WHITE_TO_MOVE) || (myPos->myBoard[i - 8] >= 0 && myPos->turn == BLACK_TO_MOVE))
		{
			myPos->moves[myPos->numberOfMoves].from = i;
			myPos->moves[myPos->numberOfMoves].to = i - 8;
			myPos->numberOfMoves++;
		}
	}
	//left
	if (fileFromCoord(i) > 1) {
		if ((myPos->myBoard[i - 1] <= 0 && myPos->turn == WHITE_TO_MOVE) || (myPos->myBoard[i - 1] >= 0 && myPos->turn == BLACK_TO_MOVE))
		{
			myPos->moves[myPos->numberOfMoves].from = i;
			myPos->moves[myPos->numberOfMoves].to = i - 1;
			myPos->numberOfMoves++;
		}
	}
	//right
	if (fileFromCoord(i) < 8) {
		if ((myPos->myBoard[i + 1] <= 0 && myPos->turn == WHITE_TO_MOVE) || (myPos->myBoard[i + 1] >= 0 && myPos->turn == BLACK_TO_MOVE))
		{
			myPos->moves[myPos->numberOfMoves].from = i;
			myPos->moves[myPos->numberOfMoves].to = i + 1;
			myPos->numberOfMoves++;
		}
	}
	//up-right
	if (rankFromCoord(i) > 1 && fileFromCoord(i) < 8) {
		if ((myPos->myBoard[i + 9] <= 0 && myPos->turn == WHITE_TO_MOVE) || (myPos->myBoard[i + 9] >= 0 && myPos->turn == BLACK_TO_MOVE))
		{
			myPos->moves[myPos->numberOfMoves].from = i;
			myPos->moves[myPos->numberOfMoves].to = i + 9;
			myPos->numberOfMoves++;
		}
	}
	//up-left
	if (rankFromCoord(i) > 1 && fileFromCoord(i) > 1) {
		if ((myPos->myBoard[i + 7] <= 0 && myPos->turn == WHITE_TO_MOVE) || (myPos->myBoard[i + 7] >= 0 && myPos->turn == BLACK_TO_MOVE))
		{
			myPos->moves[myPos->numberOfMoves].from = i;
			myPos->moves[myPos->numberOfMoves].to = i + 7;
			myPos->numberOfMoves++;
		}
	}
	//down-right
	if (rankFromCoord(i) < 8 && fileFromCoord(i) < 8) {
		if ((myPos->myBoard[i - 7] <= 0 && myPos->turn == WHITE_TO_MOVE) || (myPos->myBoard[i - 7] >= 0 && myPos->turn == BLACK_TO_MOVE))
		{
			myPos->moves[myPos->numberOfMoves].from = i;
			myPos->moves[myPos->numberOfMoves].to = i - 7;
			myPos->numberOfMoves++;
		}
	}
	//down-left
	if (rankFromCoord(i) < 8 && fileFromCoord(i) > 1) {
		if ((myPos->myBoard[i - 9] <= 0 && myPos->turn == WHITE_TO_MOVE) || (myPos->myBoard[i - 9] >= 0 && myPos->turn == BLACK_TO_MOVE))
		{
			myPos->moves[myPos->numberOfMoves].from = i;
			myPos->moves[myPos->numberOfMoves].to = i - 9;
			myPos->numberOfMoves++;
		}
	}

	//white castle-king side
	if (myPos->myBoard[60] == WHITE_KING && myPos->myBoard[61] == 0 &&  myPos->myBoard[62] == 0 &&myPos->myBoard[63] == WHITE_ROOK && myPos->turn == WHITE_TO_MOVE && myPos->whiteCastleKing == 0)
		{
			myPos->moves[myPos->numberOfMoves].from = 60;
			myPos->moves[myPos->numberOfMoves].to = 62;
			myPos->numberOfMoves++;
		}

		//white castle-queen side
	if (myPos->myBoard[60] == WHITE_KING && myPos->myBoard[59] == 0 &&  myPos->myBoard[58] == 0 && myPos->myBoard[57] == 0 && myPos->myBoard[56] == WHITE_ROOK && myPos->turn == WHITE_TO_MOVE && myPos->whiteCastleQueen == 0)
		{
			myPos->moves[myPos->numberOfMoves].from = 60;
			myPos->moves[myPos->numberOfMoves].to = 58;
			myPos->numberOfMoves++;
		}
	//black castle-king side
	if (myPos->myBoard[60] == BLACK_KING && myPos->myBoard[5] == 0 &&  myPos->myBoard[6] == 0 &&myPos->myBoard[7] == BLACK_ROOK && myPos->turn == BLACK_TO_MOVE && myPos->blackCastleKing == 0)
		{
			myPos->moves[myPos->numberOfMoves].from = 4;
			myPos->moves[myPos->numberOfMoves].to = 6;
			myPos->numberOfMoves++;
		}

		//black castle-queen side
	if (myPos->myBoard[60] == BLACK_KING && myPos->myBoard[3] == 0 &&  myPos->myBoard[2] == 0 && myPos->myBoard[1] == 0 && myPos->myBoard[0] == BLACK_ROOK && myPos->turn == BLACK_TO_MOVE && myPos->blackCastleQueen == 0)
		{
			myPos->moves[myPos->numberOfMoves].from = 4;
			myPos->moves[myPos->numberOfMoves].to = 2;
			myPos->numberOfMoves++;
		}

	
}

//mdo there is some problem with pawn take from e4-c5 and d4-e5
void findPawnMoves(position* myPos, int i)
{
	if (myPos->turn == WHITE_TO_MOVE)
	{
		if (myPos->myBoard[i - 8] == 0) //bialy pionek o jedno pole na przod
		{
			myPos->moves[myPos->numberOfMoves].from = i;
			myPos->moves[myPos->numberOfMoves].to = i - 8;
			myPos->numberOfMoves++;
		}
		if (myPos->myBoard[i - 16] == 0 && myPos->myBoard[i - 8] == 0 && (i >= 48 && i <= 55)) //bialy pionek o 2 pola na przod
		{
				myPos->moves[myPos->numberOfMoves].from = i;
				myPos->moves[myPos->numberOfMoves].to = i - 16;
				myPos->numberOfMoves++;
		}
		if (myPos->myBoard[i - 7] < 0 && isRightBound(i) == 0) //bialy pionek bije w prawo //dlaczego nie dziala?
		{
			myPos->moves[myPos->numberOfMoves].from = i;
			myPos->moves[myPos->numberOfMoves].to = i - 7;
			myPos->numberOfMoves++;
		}
		if (myPos->myBoard[i - 9] < 0 && isLeftBound(i) == 0) //bialy pionek bije w lewo
		{
			myPos->moves[myPos->numberOfMoves].from = i;
			myPos->moves[myPos->numberOfMoves].to = i - 9;
			myPos->numberOfMoves++;
		}
	}







	if (myPos->turn == BLACK_TO_MOVE)
	{
		if (myPos->myBoard[i + 8] == 0) //bialy pionek o jedno pole na przod
		{
			myPos->moves[myPos->numberOfMoves].from = i;
			myPos->moves[myPos->numberOfMoves].to = i + 8;
			myPos->numberOfMoves++;

			if (myPos->myBoard[i + 16] == 0 && myPos->myBoard[i + 8] == 0 && (i >= 8 && i <= 15)) //bialy pionek o 2 pola na przod
			{
				myPos->moves[myPos->numberOfMoves].from = i;
				myPos->moves[myPos->numberOfMoves].to = i + 16;
				myPos->numberOfMoves++;
			}
			if (myPos->myBoard[i + 9] > 0 && isRightBound(i) == 0) //bialy pionek bije w prawo
			{
				myPos->moves[myPos->numberOfMoves].from = i;
				myPos->moves[myPos->numberOfMoves].to = i + 9;
				myPos->numberOfMoves++;
			}
			if (myPos->myBoard[i + 7] > 0 && isLeftBound(i) == 0) //bialy pionek bije w lewo
			{
				myPos->moves[myPos->numberOfMoves].from = i;
				myPos->moves[myPos->numberOfMoves].to = i + 7;
				myPos->numberOfMoves++;
			}

		}
	}
}

void findRookMoves(position* myPos, int i)
{

	int j = 0;
	if (isUpBound(i) == 0) //gora
	{

		while (isUpBound(i + j) == 0)//jezeli nie jestesmy poza bandą gorna i lewa
		{
			j = j - 8;
			if ((myPos->myBoard[i + j] <= 0 && myPos->turn == WHITE_TO_MOVE) || (myPos->myBoard[i + j] >= 0 && myPos->turn == BLACK_TO_MOVE))
			{
				myPos->moves[myPos->numberOfMoves].from = i;
				myPos->moves[myPos->numberOfMoves].to = i + j;
				myPos->numberOfMoves++;
				if (ifPieceTake(myPos, i + j)) break;
			}
			if ((myPos->myBoard[i + j] > 0 && myPos->turn == WHITE_TO_MOVE) || (myPos->myBoard[i + j] < 0 && myPos->turn == BLACK_TO_MOVE)) break;
		}
	}

	j = 0;
	if (isDownBound(i) == 0) //dolne
	{

		while (isDownBound(i + j) == 0)//jezeli nie jestesmy poza bandą dolna i prawa
		{
			j = j + 8;
			if ((myPos->myBoard[i + j] <= 0 && myPos->turn == WHITE_TO_MOVE) || (myPos->myBoard[i + j] >= 0 && myPos->turn == BLACK_TO_MOVE))
			{
				myPos->moves[myPos->numberOfMoves].from = i;
				myPos->moves[myPos->numberOfMoves].to = i + j;
				myPos->numberOfMoves++;
				if (ifPieceTake(myPos, i + j)) break;
			}
			if ((myPos->myBoard[i + j] > 0 && myPos->turn == WHITE_TO_MOVE) || (myPos->myBoard[i + j] < 0 && myPos->turn == BLACK_TO_MOVE)) break;

		}
	}

	j = 0;
	if (isLeftBound(i) == 0) //gorne lewo
	{

		while (isLeftBound(i + j) == 0)//jezeli nie jestesmy poza bandą gorna i lewa
		{
			j = j - 1;
			if ((myPos->myBoard[i + j] <= 0 && myPos->turn == WHITE_TO_MOVE) || (myPos->myBoard[i + j] >= 0 && myPos->turn == BLACK_TO_MOVE))
			{
				myPos->moves[myPos->numberOfMoves].from = i;
				myPos->moves[myPos->numberOfMoves].to = i + j;
				myPos->numberOfMoves++;
				if (ifPieceTake(myPos, i + j)) break;

			}
			if ((myPos->myBoard[i + j] > 0 && myPos->turn == WHITE_TO_MOVE) || (myPos->myBoard[i + j] < 0 && myPos->turn == BLACK_TO_MOVE)) break;
		}
	}

	j = 0;
	if (isRightBound(i) == 0) //gorne lewo
	{
		while (isRightBound(i + j) == 0)//jezeli nie jestesmy poza bandą gorna i lewa
		{
			j = j + 1;
			if ((myPos->myBoard[i + j] <= 0 && myPos->turn == WHITE_TO_MOVE) || (myPos->myBoard[i + j] >= 0 && myPos->turn == BLACK_TO_MOVE))
			{
				myPos->moves[myPos->numberOfMoves].from = i;
				myPos->moves[myPos->numberOfMoves].to = i + j;
				myPos->numberOfMoves++;
				if (ifPieceTake(myPos, i + j)) break;
			}
			if ((myPos->myBoard[i + j] > 0 && myPos->turn == WHITE_TO_MOVE) || (myPos->myBoard[i + j] < 0 && myPos->turn == BLACK_TO_MOVE)) break;

		}
	}
	j = 0;
}



void findQueenMoves(position* myPos, int i)
{
	findRookMoves(myPos, i);
	findBishopMoves(myPos, i);
}

void findKnightMoves(position* myPos, int i)
{
	if (fileFromCoord(i) < 8 && rankFromCoord(i) < 7) //godzina 1
	{
		if ((myPos->myBoard[i - 15] <= 0 && myPos->turn == WHITE_TO_MOVE) || (myPos->myBoard[i - 15] >= 0 && myPos->turn == BLACK_TO_MOVE))
		{
			myPos->moves[myPos->numberOfMoves].from = i;
			myPos->moves[myPos->numberOfMoves].to = i - 15;
			myPos->numberOfMoves++;
		}
	}
	if (fileFromCoord(i) < 7 && rankFromCoord(i) < 8) //godzina 2
	{
		if ((myPos->myBoard[i - 6] <= 0 && myPos->turn == WHITE_TO_MOVE) || (myPos->myBoard[i - 6] >= 0 && myPos->turn == BLACK_TO_MOVE))
		{
			myPos->moves[myPos->numberOfMoves].from = i;
			myPos->moves[myPos->numberOfMoves].to = i - 6;
			myPos->numberOfMoves++;
		}
	}

	if (fileFromCoord(i) < 7 && rankFromCoord(i) > 1) //godzina 4
	{
		if ((myPos->myBoard[i + 10] <= 0 && myPos->turn == WHITE_TO_MOVE) || (myPos->myBoard[i + 10] >= 0 && myPos->turn == BLACK_TO_MOVE))
		{
			myPos->moves[myPos->numberOfMoves].from = i;
			myPos->moves[myPos->numberOfMoves].to = i + 10;
			myPos->numberOfMoves++;
		}
	}

	if (fileFromCoord(i) < 8 && rankFromCoord(i) > 2) //godzina 5
	{
		if ((myPos->myBoard[i + 17] <= 0 && myPos->turn == WHITE_TO_MOVE) || (myPos->myBoard[i + 17] >= 0 && myPos->turn == BLACK_TO_MOVE))
		{
			myPos->moves[myPos->numberOfMoves].from = i;
			myPos->moves[myPos->numberOfMoves].to = i + 17;
			myPos->numberOfMoves++;
		}
	}

	if (fileFromCoord(i) > 1 && rankFromCoord(i) > 2) //godzina 7
	{
		if ((myPos->myBoard[i + 15] <= 0 && myPos->turn == WHITE_TO_MOVE) || (myPos->myBoard[i + 15] >= 0 && myPos->turn == BLACK_TO_MOVE))
		{
			myPos->moves[myPos->numberOfMoves].from = i;
			myPos->moves[myPos->numberOfMoves].to = i + 15;
			myPos->numberOfMoves++;
		}
	}


	if (fileFromCoord(i) > 2 && rankFromCoord(i) > 1) //godzina 8
	{
		if ((myPos->myBoard[i + 6] <= 0 && myPos->turn == WHITE_TO_MOVE) || (myPos->myBoard[i + 6] >= 0 && myPos->turn == BLACK_TO_MOVE))
		{
			myPos->moves[myPos->numberOfMoves].from = i;
			myPos->moves[myPos->numberOfMoves].to = i + 6;
			myPos->numberOfMoves++;
		}
	}

	if (fileFromCoord(i) > 2 && rankFromCoord(i) < 8) //godzina 10
	{
		if ((myPos->myBoard[i - 10] <= 0 && myPos->turn == WHITE_TO_MOVE) || (myPos->myBoard[i - 10] >= 0 && myPos->turn == BLACK_TO_MOVE))
		{
			myPos->moves[myPos->numberOfMoves].from = i;
			myPos->moves[myPos->numberOfMoves].to = i - 10;
			myPos->numberOfMoves++;
		}
	}

	if (fileFromCoord(i) > 1 && rankFromCoord(i) < 7) //godzina 11
	{
		if ((myPos->myBoard[i - 17] <= 0 && myPos->turn == WHITE_TO_MOVE) || (myPos->myBoard[i - 17] >= 0 && myPos->turn == BLACK_TO_MOVE))
		{
			myPos->moves[myPos->numberOfMoves].from = i;
			myPos->moves[myPos->numberOfMoves].to = i - 17;
			myPos->numberOfMoves++;
		}
	}

}

void findMoves(position* myPos)
{

	int i = 0;
	myPos->numberOfMoves = 0;
	for (i = 0; i < 64; i++)
	{
		if (myPos->turn == WHITE_TO_MOVE)
		{
			if (myPos->myBoard[i] == WHITE_PAWN) findPawnMoves(myPos, i);
			if (myPos->myBoard[i] == WHITE_BISHOP) findBishopMoves(myPos, i);
			if (myPos->myBoard[i] == WHITE_ROOK) findRookMoves(myPos, i);
			if (myPos->myBoard[i] == WHITE_QUEEN) findQueenMoves(myPos, i);
			if (myPos->myBoard[i] == WHITE_KNIGHT) findKnightMoves(myPos, i);
			if (myPos->myBoard[i] == WHITE_KING) findKingMoves(myPos, i);
		}
		if (myPos->turn == BLACK_TO_MOVE)
		{
			if (myPos->myBoard[i] == BLACK_PAWN) findPawnMoves(myPos, i);
			if (myPos->myBoard[i] == BLACK_BISHOP) findBishopMoves(myPos, i);
			if (myPos->myBoard[i] == BLACK_ROOK) findRookMoves(myPos, i);
			if (myPos->myBoard[i] == BLACK_QUEEN) findQueenMoves(myPos, i);
			if (myPos->myBoard[i] == BLACK_KNIGHT) findKnightMoves(myPos, i);
			if (myPos->myBoard[i] == BLACK_KING) findKingMoves(myPos, i);

		}

	}
}