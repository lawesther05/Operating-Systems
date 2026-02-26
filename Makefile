CC = gcc
CFLAGS = -Wall -g

all: lab4

lab4: lab4.c
	$(CC) $(CFLAGS) -o lab4 lab4.c

clean:
	rm -f lab4
