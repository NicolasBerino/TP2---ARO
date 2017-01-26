CC     = gcc
CFLAGS = -Wall -std=c99

all: knapsack

knapsack:
	$(CC) -o knapsack knapsack.c

clean:
	rm -vf knapsack
