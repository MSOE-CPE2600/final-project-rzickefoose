CFLAGS=-c -g -Wall -Werror -pthread
CC=gcc

all: Client/client Server/server

Client/client: Client/client.c
Server/server: Server/server.c