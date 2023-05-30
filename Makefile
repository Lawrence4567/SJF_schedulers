CC=g++
CFLAGS=-std=c++2a -pthread

all: main

main: main.c
	$(CC) $(CFLAGS) main.c -o main

clean:
	rm -f main
