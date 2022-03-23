# chess
Tiny console chess engine
build command :
                just 'make' :-)

Usage of run:
                ./main -d [depth]  -fen [FEN STRING]
                
For example:
                ./main -d 5 -fen "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
               
Recommended depth parameter is 4. Bigger numbers may effect with time consuming analysis.
FEN_STRING makes the game start from given position. Lack of this parameter will make the game run from initial position.
Link to FEN str generator https://lichess.org/editor for more information.


Missing features to do 
1. En-passant
2. piece promotion
3. Check-if-check (taking the king is possible so far ;( )
4. alpha-beta pruning
5. hash table to implement, to avoid running on repeated positions.
6.
7.
8.
9.

