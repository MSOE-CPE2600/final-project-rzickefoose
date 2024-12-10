#define PORT 8080
#define BUFFER_SIZE 1024
#define BACKLOG 2
#define USER_COUNT 5

#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <asm-generic/socket.h>
#include <sys/ipc.h>
#include <sys/shm.h>