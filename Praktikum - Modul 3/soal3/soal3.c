// untuk soal 3
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pthread.h>
#include <dirent.h>
#include <stdlib.h>
#include <ctype.h>

pthread_t tid[1000];
int t1 = 0;
char data[1000][1000];

void createDir(char *name) {
    int dir = mkdir(name, 0777);
}

void *processFiles(void *loc) {
    char *fileloc, *ext, *filename, *token;
    char str1[100], str2[100], ext1[50], fname[100];
    char fileloc2[1000];

    struct dirent *dp;
    fileloc = (char *) loc;
    //printf("%s\n", fileloc);
    DIR *dir = opendir(fileloc);

    if (dir == NULL) {
        strcpy(str1, fileloc);
        ext = strrchr(str1, '/');
        token = strtok(str1, ".");
        token = strtok(NULL, "");
        if (ext != NULL) {
            if (ext[1] == "." && token != NULL) {
                strcpy(ext1, "hidden");
            } else if (token == NULL) {
                strcpy(ext1, "unknown");
            } else {
                strcpy(ext1, token);
                for (int i = 0; i < strlen(ext1); i++) {
                    ext1[i] = tolower(ext1[i]);
                }
            }
        }
        
        strcpy(str2, fileloc);
        filename = strrchr(str2, '/');
        token = strtok(filename, "/");
        token = strtok(token, ".");
        strcpy(fname, token);

        createDir(ext1);

        strcpy(fileloc2, fileloc);
        // printf("%s\n", fileloc);
        char temp[1000];
        if (strcmp(ext1, "unknown") == 0 || strcmp(ext1, "hidden") == 0) {
            strcpy(temp, ext1);
            strcat(temp, "/");
            strcat(temp, fname);
        } else {
            strcpy(temp, ext1);
            strcat(temp, "/");
            strcat(temp, fname);
            strcat(temp, ".");
            strcat(temp, ext1);
        }
        rename(fileloc2, temp);
        // printf("%s %s %s\n", ext1, fname, temp);
    }
}

void listFilesRecursively(char *basePath) {
    char path2[1000];
    struct dirent *dp;
    DIR *dir = opendir(basePath);

    if (!dir) {
        return;
    }

    while ((dp = readdir(dir)) != NULL) {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0) {
            strcpy(path2, basePath);
            //printf("ini path2 sebelum: %s\n", path2);
            //if (strcmp(basePath, path2) != 0) {
            //	strcat(path2, "/");
            //}
            strcat(path2, dp->d_name);
            if (dp->d_type == DT_DIR) {
            	strcat(path2, "/");	
            }
            //printf("ini path2: %s\n", path2);
            //printf("ini dpname: %s\n", dp->d_name);
            char buffer[1000];
            strcpy(buffer, dp->d_name);
            if (buffer[0] == '.') {
                createDir("hidden");
                char hidd[1000];
                strcpy(hidd, "hidden/");
                strcat(hidd, dp->d_name);
                rename(path2, hidd);
                // printf("%s\n", hidd);
            } else {
                char loc[1000];
                strcpy(loc, path2);
                //printf("%s\n", loc);
                strcpy(data[t1], loc);
                //printf("%s\n", data[t1]);
                t1++;
            }
            listFilesRecursively(path2);
        }
    }
    closedir(dir);
}

int main() {
    //unzip manual
    char path[] = "/home/ubuntu/shift3/hartakarun/";

    listFilesRecursively(path);

    for (int i = 0; i < t1; i++) {
        pthread_create(&(tid[i]), NULL, processFiles, (char*)data[i]);
    }
    for(int i = 0; i < t1; i++){
        pthread_join(tid[i],NULL);
    }
}
