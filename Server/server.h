/***************************************************************** 
* Filename: server.h
* Description: Header file for server.c
* Author: Rose Zickefoose and Keegan Kastman
* Date: 12/9/24
* Note (Compile Instructions): make
*****************************************************************/

#define PORT 8080
#define BUFFER_SIZE 1024
#define BACKLOG 2
#define USER_COUNT 1
#define IP "127.0.0.1"

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