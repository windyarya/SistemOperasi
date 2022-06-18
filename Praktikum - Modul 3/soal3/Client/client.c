// untuk soal 3 client
#include <stdio.h>
#include <sys/socket.h>
#include <sys/wait.h>
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

void zipping(char *pathz) {
    int status;
    pid_t child_id;
    child_id = fork();
    if (child_id == 0) {
        char *argv[] = {"zip", "-r", "hartakarun.zip", pathz, NULL};
        execv("/usr/bin/zip", argv);
    } else {
        ((wait(&status)) > 0);
    }
}

int send_file(int socket, char *filename) {
    char buffer[MAX_LENGTH] = {0};
    char filepath[100] = "./";
    strcat(filepath, filename);
    // printf("%s %s", filepath, filename);
    FILE *fptr = fopen(filepath, "r");
    if (fptr == NULL) {
        printf("File not found\n");
        return -1;
    }
    int f = 0;
    bzero(buffer, MAX_LENGTH);
    // printf("%s\n", buffer);
    // fseek(fptr, 0L, SEEK_END);
    // long int res = ftell(fptr);
    // printf("%ld", res);
    int filesize;
    filesize = fread(buffer, sizeof(char), MAX_LENGTH, fptr);
    // printf("%d\n", filesize);
    int t = 0;
    long int temp = 0;
    size_t am;
    while (filesize > 0) {
        am = send(socket, buffer, filesize, 0);
        // printf("%d: filesize: %d, sendsocket: %ld\n", t, filesize, am);
        if (am <= 0) {
            // printf("Masuk sini");
            // printf("%d: filesize: %d, sendsocket: %ld\n", t, filesize, am);
            fprintf(stderr, "Failed to send file (errno = %d)\n", errno);
            f = 1;
        }
        t++;
        bzero(buffer, MAX_LENGTH);
        // printf("buffer sesudah bzero: %s\n", buffer);
        temp += filesize;
        // if (am == -1) {
        //     printf("%ld %d %d %d", am, socket, filesize, buffer);
        // }
        filesize = fread(buffer, sizeof(char), MAX_LENGTH, fptr);
        // printf("%ld\n", am);
    }
    // printf("Masuk sini");
    // printf("%ld", temp);
    fclose(fptr);
    return f;
}

void *sendZip(void *arg) {
    char buffer[1024] = {0};
    bzero(buffer, MAX_LENGTH);
    fgets(buffer, MAX_LENGTH, stdin);
    buffer[strcspn(buffer, "\n")] = 0;
    // printf("%s\n", buffer);
    // printf("%d\n", udata.socket);
    send(udata.socket, buffer, MAX_LENGTH, 0);

    char cl[MAX_LENGTH];
    strcpy(cl, buffer);
    char *com = strtok(cl, " ");
    // printf("%s\n", com);

    for (int i = 0; i < strlen(com); i++) {
        com[i] = tolower(com[i]);
    }

    if (strcmp("send", com) == 0) {
        char *filename;
        com = strtok(NULL, " ");
        // printf("%s\n", com);
        filename = com;
        // printf("%s\n", filename);
        strcpy(udata.file, filename);
        // printf("%s\n", udata.file);
        if (send_file(udata.socket, filename) == 0) {
            printf("File sent\n");
        } else {
            printf("Sending failed\n");
        }
    }
}

int main() {
    char pathz[] = "/home/ubuntu/shift3/hartakarun";
    zipping(pathz);
    
    struct sockaddr_in address;
    int sock = 0;
    int valread, ineta, inetc;
    struct sockaddr_in serv_addr;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        printf("Socket creation failed\n");
        return -1;
    }

    bzero(&serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    ineta = inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);
    if (ineta <= 0) {
        printf("Invalid address\n");
        return -1;
    }

    inetc = connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if (inetc < 0) {
        printf("Connection failed\n");
        return -1;
    } else {
        udata.socket = sock;
        printf("Connected to server with the address %d\n", sock);
    }

    pthread_t tid;
    pthread_create(&(tid), NULL, &sendZip, (void *)&sock);
    pthread_join(tid, NULL);
}