main: main.c findMoves.c
	gcc -o main main.c moves.c findMoves.c -I. -lpthread -lcrypto
clean:
	rm -f main

