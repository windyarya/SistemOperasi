// untuk soal 3 server
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

#define PORT 8080
#define MAX_LENGTH 1024

struct user {
    int socket;
    char input[100];
    char file[100];
} udata;

int error(char *err) {
    perror(err);
    exit(EXIT_FAILURE);
}

int recv_file(int socket, char *fname) {
    char buffer[MAX_LENGTH] = {0};
    char filename[MAX_LENGTH];
    strcpy(filename, udata.file);
    // printf("%d\n", socket);
    // printf("%s\n", fname);
    // printf("%s\n", filename);
    // printf("%s\n", udata.file);
    int flag = 0;
    char fpath[MAX_LENGTH] = "./hartakarun.zip";
    FILE *fptr = fopen(fpath, "wb");
    if (fptr == NULL) {
        printf("File %s cannot be made in the server\n", fname);
        flag = 1;
    } else {
        bzero(buffer, MAX_LENGTH);
        int fsize = 0;
        while ((fsize = recv(socket, buffer, MAX_LENGTH, 0)) > 0) {
            // printf("%d\n", fsize);
            int wsize = fwrite(buffer, sizeof(char), fsize, fptr);
            // printf("%d\n", wsize);
            if (wsize < fsize) {
                error("Failed to write on the server");
                flag = 1;
            }
            bzero(buffer, MAX_LENGTH);
            if (fsize == 0 || fsize != MAX_LENGTH) {
                break;
            }
        }
        if (fsize < 0) {
            if (errno == EAGAIN) {
                printf("Timeout\n");
            } else {
                fprintf(stderr, "Failure = %d\n", errno);
                exit(1);
            }
            flag = 1;
        }
        printf("Receiving file from client\n");
    }
    fclose(fptr);
    printf("File %s from client successfully accepted\n", udata.file);
    return flag;
}

void *recvZip() {
    char buffer[1024];
    int f;
    read(udata.socket, buffer, 1024);
    // printf("%d\n", udata.socket);
    // printf("%s\n", buffer);
    // printf("%s\n", udata.file);
    f =  recv_file(udata.socket, udata.file);
    if (f == 1) {
        printf("File send failed\n");
    } else {
        printf("File sent successfully\n");
    }
}

int main() {
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    int valread, ineta, inetc, serv_sock, clientsocket;
    char buffer[1024] = {0};

    serv_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (serv_sock < 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("Setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(serv_sock, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    } else {
        printf("Server is running\n");
    }

    if (listen(serv_sock, 1) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    if ((clientsocket = accept(serv_sock, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    } else {
        printf("Connected with client with the address: %d\n", clientsocket);
        udata.socket = clientsocket;
    }

    pthread_t tid;
    pthread_create(&(tid), NULL, &recvZip, 0);
    pthread_join(tid, NULL);
}