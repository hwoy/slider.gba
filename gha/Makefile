CC = gcc
BIN=gha

.PHONY: all clean

all: gha.c
	$(CC) -ansi -pedantic -Wall -O2 -o $(BIN) gha.c

clean:
	rm -rf *.exe *.o gha
	