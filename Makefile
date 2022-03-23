main: main.c findMoves.c
	gcc -o main main.c moves.c findMoves.c -I.
clean:
	rm -f main

