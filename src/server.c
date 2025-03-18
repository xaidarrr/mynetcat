#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <pthread.h>

#define BUFFER_SIZE 1024

int receive_file(int sockfd) {
    char filename[BUFFER_SIZE];
    ssize_t filename_len = recv(sockfd, filename, BUFFER_SIZE, 0);
    if (filename <= 0) {
        perror("Receiving");
        return -1;
    }
    filename[filename_len] = '\0';

    int filefd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (filefd == -1) {
        perror("File");
        return -1;
    }

    char buffer[BUFFER_SIZE];
    ssize_t bytes_received;
    while ((bytes_received = recv(sockfd, buffer, BUFFER_SIZE, 0)) > 0) {
        if (write(filefd, buffer, bytes_received) == -1) {
            perror("File");
            close(filefd);
            return -1;
        }
    }
    close(filefd);
    printf("File %s received successfully\n", filename);
    return 0;
}

void *handle_client(void *arg) {
    printf("Server connected. Receiving file...\n");
    int client_socket = *(int *)arg;
    free(arg);
    receive_file(client_socket);
    close(client_socket);
    return NULL;
}

int start_server(int port) {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Socket");
        return -1;
    }
    
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Bind");
        close(server_socket);
        return -1;
    }

    if (listen(server_socket, 5) == -1) {
        perror("Listen");
        close(server_socket);
        return -1;
    }

    printf("Server is listening on port %d\n", port);
    
    while(1) {
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_len);
        if (client_socket == -1) {
            perror("Accept");
            continue;
        }
        
        pthread_t thread;
        int *new_socket = malloc(sizeof(int));
        *new_socket = client_socket;
        if (pthread_create(&thread, NULL, handle_client, (void *)new_socket) != 0) {
            perror("Thread");
            free(new_socket);
        }

        pthread_detach(thread);
    }

    close(server_socket);
    return 0;
} 
