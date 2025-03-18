#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/client.h"
#include "../include/server.h"

void manual(const char *progname) {
    printf("Usage:\n");
    printf(" %s -c <IP> <PORT>  # Client mode\n", progname);
    printf(" %s -l <PORT>       # Server mode\n", progname);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        manual(argv[0]);
    }

    if (strcmp(argv[1], "-c") == 0 && argc >= 4) {
        printf("Starting in client mode...\n");
        if (argc == 4) {
            start_client(argv[2], atoi(argv[3]), NULL);
        } else {
            start_client(argv[2], atoi(argv[3]), argv[4]);
        }
    }
    else if (strcmp(argv[1], "-l") == 0 && argc == 3) {
        printf("Starting in server mode...\n");
        start_server(atoi(argv[2]));
    }
    else {
        manual(argv[0]);
    }

    return 0;
} 
