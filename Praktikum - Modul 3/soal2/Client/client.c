#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <errno.h>
#include <dirent.h>
#include <ctype.h>

#define MAX_LENGTH 1024
#define PORT 8080

struct user_t {
    int is_auth;
    int socket;
    char file[1000];
    char input[1000];
    char mode[1000];
} user;

int error(char *err) {
    perror(err);
    exit(EXIT_FAILURE);
}

void *printing(void *arg) {
    if (strcmp(user.mode, "recvstrings") == 0) {
        int sock = *(int *)arg;
        char buffer[1024] = {0};
        while (1) {
            memset(buffer, 0, 1024);
            if (recv(sock, buffer, 1024, 0) > 1) {
                char buffer2[1024];
                strcpy(buffer2, buffer);
                char *token = strtok(buffer2, "\n");
                printf("%s", buffer);
            }
        }
    }
}

void *input_user(void *arg) {
    while (strcmp(user.mode, "recvstrings") == 0) {
        char buffer[1024] = {0};
        bzero(buffer, MAX_LENGTH);
        fgets(buffer, MAX_LENGTH, stdin);
        buffer[strcspn(buffer, "\n")] = 0;

        send(user.socket, buffer, MAX_LENGTH, 0);
    }
}

int main(int argc, char const *argv[]) {
    pthread_t input, print;
    struct sockaddr_in address;
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    } else {
        user.socket = sock;
        printf("Connected to server with the address %d\n", sock);
    }
    strcpy(user.mode, "recvstrings");

    pthread_create(&print, NULL, &printing, (void *)&sock);
    pthread_create(&input, NULL, &input_user, (void *)&sock);
    while (1) {
        if (pthread_join(input, NULL) == 0) {
            pthread_create(&input, NULL, &input_user, (void *)&sock);
        }
    }
    if (strcmp(user.mode, "recvstrings") == 0) {
        pthread_join(print, NULL);
    } else {
        pthread_exit(&print);
    }
}