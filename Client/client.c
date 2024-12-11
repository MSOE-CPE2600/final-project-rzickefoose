/***************************************************************** 
* Filename: client.c
* Description: Client side of the chatroom program
* Author: Rose Zickefoose and Keegan Kastman
* Date: 12/9/24
* Note (Compile Instructions): make
*****************************************************************/

#include "client.h"

void connect_to_server() {
    // Initializes Variables
    int fd, status;
    struct sockaddr_in addr;
    char buffer[BUFFER_SIZE] = {0};

    // Creates Socket File Descriptor
    if((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket Failed");
        exit(EXIT_FAILURE);
    }

    // Defines the address
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);

    // Connects the address
    if(inet_pton(AF_INET, "192.168.1.137", &addr.sin_addr) <= 0) {
        perror("Invalid Address");
        exit(EXIT_FAILURE);
    }

    // Connects to Server
    if((status = connect(fd, (struct sockaddr *)&addr, sizeof(addr))) < 0) {
        perror("Connection Failed");
        exit(EXIT_FAILURE);
    } 

    while(1) {
        // Gets the message from user
        if(fgets(buffer, BUFFER_SIZE, stdin) == NULL) {
            perror("fgets() Failed");
            exit(EXIT_FAILURE);
        }

        if(send(fd, buffer, BUFFER_SIZE, 0) < 0) {
            perror("Send Failed");
            exit(EXIT_FAILURE);
        }

        if(strcmp(buffer, "/quit\n") == 0) {
            exit(1);
        }

        // Recieves Data From Server
        char buf[BUFFER_SIZE];
        recv(fd, buf, BUFFER_SIZE, 0);
        printf("From server: %s", buf);

        memset(buffer, 0, BUFFER_SIZE);
    }

    close(fd);
}

int main(void) {
    connect_to_server();
}