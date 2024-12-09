CFLAGS=-c -g -Wall -Werror
CC=gcc

all: client server

client: client.o
	$(CC) -o client client.o

server: server.o
	$(CC) -o server server.o

clean:
	rm -f *.o client server

%.o: %.c
	$(CC) $(CFLAGS) -o $@ $<