CC = gcc
GOL = gol
GOLTHREAD = gol-threaded
CFLAGS = -g -Wall -Wextra -std=c99 -D_GNU_SOURCE -pthread
CFLAGS += -lSDL

GOL_OBJS = gol.c gol-threaded.c

all: gol gol-threaded

gol: gol.c
	$(CC) $(CFLAGS) -o $(GOL) gol.c

gol-threaded: gol-threaded.c
	$(CC) $(CFLAGS) -o $(GOLTHREAD) gol-threaded.c

clean: 
	rm -f gol gol.o
