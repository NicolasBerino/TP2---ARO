CC     = gcc
CFLAGS = -g3 -std=c99

all: knapsack

.c.o :
	$(CC) -c $(CFLAGS) $*.c

knapsack : %: %.o
	$(CC) -o $@ $@.o

clean:
	rm -vf *.o knapsack
