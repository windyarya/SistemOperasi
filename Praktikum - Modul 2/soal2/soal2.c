#include <stdio.h>
#include <stdlib.h>
#include <wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>

char pathori[] = "/home/ubuntu/shift2/drakor/";

void createDir(char *dirName) {
    int status;
    pid_t child_id;
    child_id = fork();
    if (child_id == 0) {
        char *argv[] = {"mkdir", "-p", dirName, NULL};
        execv("/usr/bin/mkdir", argv);
    } else {
        ((wait(&status)) > 0);
    }
}

void unzip() {
    int status;
    pid_t child_id;
    child_id = fork();
    if (child_id == 0) {
        char *argv[] = {"unzip", "drakor.zip", "*.png", "-d", "/home/ubuntu/shift2/drakor", NULL};
        execv("/usr/bin/unzip", argv);
    } else {
        ((wait(&status)) > 0);
    }
}

void copyFiles(char *src, char *dest) {
    int status;
	pid_t child_id;
    child_id = fork();
	if (child_id == 0) {
		char *argv[] = {"cp", "-n", src, dest, NULL};
		execv("/usr/bin/cp", argv);
	} else {
		((wait(&status)) > 0);
	}
}

void deleteFile(char *namaFile) {
    int status;
    pid_t child_id;
    child_id = fork();
    if (child_id == 0) {
        char *argv[] = {"rm", namaFile, NULL};
        execv("/bin/rm", argv);
    } else {
        ((wait(&status)) > 0);
    }
}

void checkFiles(char *basePath) {
    char path[1000], title[50][100], year[50][100], genre[50][100], titleori[50][100];
    struct dirent *dp;
    DIR *dir = opendir(basePath);
    char *token;

    if (!dir) {
        return;
    }

    int data = 0;
    while ((dp = readdir(dir)) != NULL) {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0) {
            char temp[100], file[2][100];
            char *token;
            strcpy(titleori[data], dp->d_name);
            int flag = 0;
            if (!(strstr(dp->d_name, "_"))) {
                strcpy(temp, dp->d_name);
                token = strtok(temp, ";");
                strcpy(title[data], token);
                flag++;
                while (flag < 3) {
                    token = strtok(NULL, ";");
                    if (strstr(token, "20")) {
                        strcpy(year[data], token);
                        // printf("%s\n", year[data]);
                    } else if (strstr(token, ".png")) {
                        strcpy(genre[data], token);
                        // printf("%s\n", genre[data]);
                    }
                    flag++;
                }
            } else if (strstr(dp->d_name, "_")) {
                strcpy(temp, dp->d_name);
                token = strtok(temp, "_");
                strcpy(file[0], token);
                token = strtok(NULL, "_");
                strcpy(file[1], token);
                // printf("%s\n", file[0]);
                // printf("%s\n", file[1]);
                for (int i = 0; i < 2; i++) {
                    strcpy(temp, file[i]);
                    token = strtok(temp, ";");
                    strcpy(title[data], token);
                    // printf("%s\n", title[data]);
                    flag++;
                    while (flag < 3) {
                        token = strtok(NULL, ";");
                        if (strstr(token, "20")) {
                            strcpy(year[data], token);
                            // printf("%s\n", year[data]);
                        } else if (flag == 2) {
                            strcpy(genre[data], token);
                            // printf("%s\n", genre[data]);
                        }
                        flag++;
                    }
                    // printf("%s\n", title[i]);
                    flag = 0;
                    data++;
                }
            }
        }
        data++;
    }

    for (int i = 0; i < 25; i++) {
        char *token1;
        char path2[100];
        strcpy(path2, pathori);
        //menghapus substring .png pada array genre
        if (strstr(genre[i], ".png")) {
            token1 = strtok(genre[i], ".");
            strcpy(genre[i], token1);
        }

        //membuat directory
        // printf("%s\n", genre[i]);
        if (strlen(genre[i]) > 4) {
            strcat(path2, genre[i]);
            strcat(path2, "/");
            createDir(path2);
        }
        //Revisi soal 2 - untuk masukin semua file ke dalam folder genre masing-masing
        for (int j = 0; j < 25; j++) {
            if (strstr(titleori[j], title[i])) {
                char src[100], dest[100];
                strcpy(src, pathori);
                strcpy(dest, pathori);
                strcat(src, titleori[j]);
                strcat(dest, genre[i]);
                strcat(dest, "/");
                strcat(dest, title[i]);
                strcat(dest, ".png");
                // printf("%s\n", dest);
                copyFiles(src, dest);
            }
        }
    }

    // sorting all array
    char temptitle[100];
    char tempyear[100];
    char tempgenre[100];
    for (int i = 0; i < 25; i++) {
        for (int j = 0; j < 25 - 1 - i; j++) {
            if (strcmp(year[j], year[j+1]) > 0) {
                strcpy(temptitle, title[j]);
                strcpy(tempgenre, genre[j]);
                strcpy(tempyear, year[j]);
                strcpy(title[j], title[j+1]);
                strcpy(genre[j], genre[j+1]);
                strcpy(year[j], year[j+1]);
                strcpy(title[j+1], temptitle);
                strcpy(genre[j+1], tempgenre);
                strcpy(year[j+1], tempyear);
            }
        }
    }

    //Revisi Soal 2 - bagian e membuat file txt
    FILE *fptr1;
    int flagf = 0;
    char fname1[100];
    char temp[100];
    strcpy(temp, genre[0]);
    for (int i = 0; i < 8; i++) {
        char ttemp[10][50];
        int tyear[10];
        strcpy(fname1, pathori);
        strcat(fname1, "/");
        strcat(fname1, temp);
        strcat(fname1, "/");
        strcat(fname1, "data.txt");
        fptr1 = fopen(fname1, "a+");
        fprintf(fptr1, "kategori : %s", temp);
        for (int j = 0; j < 25; j++) {
            if (strcmp(genre[j], temp) == 0) {
                fprintf(fptr1, "\n\nnama : %s\n",title[j]);
                fprintf(fptr1, "rilis : tahun %s", year[j]);
                strcpy(genre[j], "do");
            }
        }
        int flag = 0;
        while (flag < 25) {
            if (strlen(genre[flag]) > 3) {
                strcpy(temp, genre[flag]);
            }
            flag++;
        }
    }

    // Revisi soal 2 - untuk delete file
    for (int j = 0; j < 25; j++) {
        char file[100];
        strcpy(file, pathori);
        strcat(file, titleori[j]);
        deleteFile(file);
    }
    
    // hapus file data.txt yg ada di home directory
    char file5[100];
    strcpy(file5, pathori);
    strcat(file5, "data.txt");
    deleteFile(file5);

    closedir(dir);
}

int main() {
    createDir(pathori);
    unzip();
    checkFiles(pathori);
    return 0;
}
