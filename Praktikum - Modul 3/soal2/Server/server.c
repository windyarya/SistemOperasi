#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <dirent.h>
#include <time.h>

#define PORT 8080
#define MAX_LENGTH 1024

int error(char *err) {
    perror(err);
    exit(EXIT_FAILURE);
}

struct user_t {
    char name[1000];
    char pwd[1000];
    char file[1000];
    char mode[1000];
    int is_auth;
    int socket;
} user;

void message(char input[]) {
    char buffer[1024];
    sprintf(buffer, "\n%s\n", input);
    send(user.socket, buffer, 1024, 0);
}

int login(char id[], char password[]) {
    FILE *fp = fopen("users.txt", "r");
    int is_auth = 0;
    char buffer[1024];
    while (fgets(buffer, 1024, fp) != NULL && is_auth == 0) {
        char file_id[1024], file_password[1024];
        char *token = strtok(buffer, ":");
        strcpy(file_id, token);
        token = strtok(NULL, "\n");
        strcpy(file_password, token);

        if (strcmp(id, file_id) == 0 && strcmp(password, file_password) == 0) {
            is_auth = 1;
        } else {
            is_auth = 0;
        }
    }
    fclose(fp);
    return is_auth;
}

int checkPass(char pass[]) {
    int flaga = 0;
    int flagb = 0;
    int flagc = 0;
    if (strlen(pass) < 6) {
        return 1;
    } else {
        for (int i = 0; i < strlen(pass); i++) {
            if (isupper(pass[i])) {
                flaga++;
            } 
            if (islower(pass[i])) {
                flagb++;
            } 
            if (isdigit(pass[i])) {
                flagc++;
            }
        }
    }
    if (flaga == 0 || flagb == 0 || flagc == 0) {
        return 1;
    }
    return 0;
}

int regist(char id[], char password[]) {
    int cp;
    int flag = 0;
    FILE *fp = fopen("users.txt", "a+"); 
    if (fp == NULL) {
        printf("masuk");
        cp = checkPass(password);
        if (cp == 0) {
            fprintf(fp, "%s:%s\n", id, password);
        } else {
            flag = 2;
        }
    } else {
        char buffer[1024];
        // fgets(buffer, 1024, fp);
        // fscanf(fp, "%s", buffer);
        // printf("%s\n", buffer);
        while (fgets(buffer, 1024, fp) != NULL) {
            char file_id[1024];
            char *token = strtok(buffer, ":");
            strcpy(file_id, token);
            // printf("%s\n", file_id);

            if (strcmp(id, file_id) == 0) {
                message("Username already exist.");
                flag = 1;
                break;
            }
        }
        if (checkPass(password) == 1) {
            message("Password Invalid. Must have be at least 6 characters long, 1 lowercase, 1 uppercase, and 1 number");
            flag = 2;
        } else if (flag == 0) {
            fprintf(fp, "%s:%s\n", id, password);
        }
    }
    fclose(fp);
    return flag;
}

void *signin() {
    char buffer[1024];
    while (1) {
        if (user.is_auth == 0) { 
            message("1. Login\n""2. Register\n""Choices: ");

            read(user.socket, buffer, 1024); 
            for (int i = 0; buffer[i]; i++) { 
                buffer[i] = tolower(buffer[i]);
            }
            if (strcmp(buffer, "login") == 0 || strcmp(buffer, "1") == 0) {
                char id[1024];
                char password[1024];
                message("\e[1;1H\e[2J");
                message("Id: ");
                read(user.socket, id, 1024);

                message("Password: ");
                read(user.socket, password, 1024);

                user.is_auth = login(id, password); 
                if (user.is_auth == 0) { 
                    message("\e[1;1H\e[2J");
                    message("Login failed id/password is wrong!");
                    printf("Login failed id/password is wrong!\n");
                    break;
                } else if (user.is_auth == 1) {
                    strcpy(user.name, id);
                    strcpy(user.pwd, password);
                    strcpy(user.mode, "recvstrings");
                    // loginsukses();
                }
            }
            if (strcmp(buffer, "register") == 0 || strcmp(buffer, "2") == 0) {
                char id[1024];
                char password[1024];
                int rs;
                message("\e[1;1H\e[2J");
                message("ID: ");
                read(user.socket, id, 1024);

                message("Password: ");
                read(user.socket, password, 1024);

                rs = regist(id, password); 
                // printf("%d\n", rs);
                if (rs == 1) { 
                    message("\e[1;1H\e[2J");
                    message("Username already exist!");
                    printf("Username already exist!\n");
                    break;
                } else if (rs == 2) {
                    message("\e[1;1H\e[2J");
                    message("Password Invalid! Must have be at least 6 characters long, 1 uppercase, 1 lowercase, and 1 number");
                    printf("Password Invalid! Must have be at least 6 characters long, 1 uppercase, 1 lowercase, and 1 number\n");
                    break;
                } else {
                    message("\e[1;1H\e[2J");
                    message("Register Successfully!");
                    printf("Register Successfully!\n");
                    break;
                }
            }
        } else if (user.is_auth == 1) { 
            // loginsukses();
        }
    }
}

int main() {
    pthread_t input;
    int server_fd, clientsocket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    } else {
        printf("Server is running\n");
    }

    if (listen(server_fd, 1) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    if ((clientsocket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    } else {
        printf("Connected with client with the address: %d\n", clientsocket);
        user.socket = clientsocket;
    }
    user.is_auth = 0;

    pthread_create(&input, NULL, &signin, 0);
    while (1) { 
        if (pthread_join(input, NULL) == 0) {
            pthread_create(&input, NULL, &signin, 0);
        }
    }
}