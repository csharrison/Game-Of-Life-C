CC = gcc
GOL = gol
CFLAGS = -g -Wall -Wextra -std=c99 -pthread -D_GNU_SOURCE
CFLAGS += -lSDL

GOL_OBJS = gol.c

gol: $(GOL_OBJS)
	$(CC) $(CFLAGS) -o $(GOL) $(GOL_OBJS)

clean: 
	rm -f gol gol.o
