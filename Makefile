EXEC = hash
CC = gcc
CFLAGS = -g -std=c99 -Wall -Wconversion -Wno-sign-conversion -Werror 

all: 
	$(CC) $(CFLAGS) -o $(EXEC) main.c hash.c testing.c pruebas_catedra.c

clean: 
		rm $(EXEC) *.o

run: all

	valgrind --leak-check=full --track-origins=yes --show-reachable=yes --error-exitcode=2 ./hash
