/***************************************************************** 
* Filename: server.c
* Description: Server side of the chatroom program
* Author: Rose Zickefoose and Keegan Kastman
* Date: 12/9/24
* Note (Compile Instructions): make
*****************************************************************/

#include "server.h"

// Struct for threading
typedef struct PARAMETERS {
    int fd, new_socket, addrlen;
    struct sockaddr_in address;
    short quit;
    int new_sock_arr[USER_COUNT];
} params;

void receive_data(void* thread_info) {
    params* p = (params*) thread_info;

    // Listening
    if(listen(p->fd, BACKLOG) < 0) {
        perror("Listen Failed");
        exit(EXIT_FAILURE);
    }

    int pid1;
    while(1) {
        // Accepts New Connection
        if((p->new_socket = accept(p->fd, (struct sockaddr *)&p->address, (socklen_t *)&p->addrlen)) < 0) {
            perror("Accept Failed");
            exit(EXIT_FAILURE);
        }

        printf("Connection Successful\n");

        pid1 = fork();

        if(pid1 == 0) {
            // Outputs data
            close(p->fd);
            ssize_t value;
            char buffer[BUFFER_SIZE];
            while((value = read(p->new_socket, buffer, BUFFER_SIZE)) > 0){
                if(strcmp(buffer, "/quit\n") == 0) { // Checks if user quit
                    printf("User quitting...\n");
                    // TEST NEW QUIT FUNCTIONALITY NEXT TIME!!!!!!!!!!
                    p->quit = 1;
                    exit(0);
                }
                printf("Client: %s", buffer);
                send(p->new_socket, buffer, BUFFER_SIZE, 0);
                memset(buffer, 0, sizeof(buffer));
            }
            close(p->new_socket);
            exit(0);
        } else {
            close(p->new_socket);
            // Listening While Other Process Handles Data
            if(listen(p->fd, BACKLOG) < 0) {
                perror("Listen Failed");
                exit(EXIT_FAILURE);
            }
        }
    }
}

void establish_connection(int new_socket[USER_COUNT]) {
    // Initializes variables
    int fd;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    // Creates Socket File Descriptor
    if((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        // Handles Errors
        perror("Socket Failed");
        exit(EXIT_FAILURE);
    }

    // Sets Socket Options
    if(setsockopt(fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) < 0) {
        // Handles Errors
        perror("Set Socket Options Failed");
        exit(EXIT_FAILURE);
    }

    // Defines the address
    address.sin_family = AF_INET; // IPv4
    address.sin_addr.s_addr = INADDR_ANY; // Any Address is permitted
    address.sin_port = htons(PORT); // Sets port to a uint16_t

    // Binds the socket to the address and port
    if(bind(fd, (struct sockaddr *)&address, addrlen) < 0) {
        // Handles Errors
        perror("Bind Failed");
        exit(EXIT_FAILURE);
    }

    printf("Listening on Port: %d\n", PORT);

    params parameters;
    parameters.fd = fd;
    parameters.addrlen = addrlen;
    parameters.address = address;
    parameters.quit = 0;
    memcpy(parameters.new_sock_arr, new_socket, USER_COUNT);


    while(1) {
        // Creates Threads
        pthread_t thread_id[USER_COUNT];
        for(int i = 0; i < USER_COUNT; i++) {
            parameters.new_socket = new_socket[i];
            pthread_create(&thread_id[i], NULL, (void *)&receive_data, (void*) &parameters);
        }
        // Joins Threads
        for(int i = 0; i < USER_COUNT; i++) {
            pthread_join(thread_id[i], NULL);
        }

        if(parameters.quit) {
            close(fd);
        }
    }

    // Close the Socket
    close(fd);
}

int main(void) {
    int new_socket[USER_COUNT];
    /* Add set_username() here */

    // Establishes connection with the cliens and handles their data
    establish_connection(new_socket);

    return 0;
}