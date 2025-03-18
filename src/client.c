#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>

#define BUFFER_SIZE 1024

int send_file(int sockfd, const char *filename) {
    int filefd = open(filename, O_RDONLY);

    if (filefd == -1) {
        perror("File");
        return -1;
    }

    size_t filename_len = strlen(filename) + 1; 
    if (send(sockfd, filename, filename_len, 0) == -1) {
        perror("Sending");
        close(filefd);
        return -1;
    }

    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;

    while((bytes_read = read(filefd, buffer, BUFFER_SIZE) > 0)) {
        if (send(sockfd, buffer, bytes_read, 0) == -1) {
            perror("Sending");
            close(filefd);
            return -1;
        }
    }

    close(filefd);
    printf("File send successfully\n");
    
    return 0;
}

int start_client(const char *ip, int port, const char *filename) {
    int sockfd;
    struct sockaddr_in server_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket");
        return -1;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, ip, &server_addr.sin_addr) <= 0) {
        perror("IP");
        close(sockfd);
        return -1;
    }
    
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Connection");
        close(sockfd);
        return -1;
    }

    printf("Connected to %s:%d\n", ip, port);

    if (filename) {
        if(send_file(sockfd, filename) == -1) {
            close(sockfd);
            return -1;
        }
    }
    

    close(sockfd);
    return 0;

}
