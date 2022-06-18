// untuk soal 1
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <ctype.h>

char path[] = "/home/ubuntu/modul3/";
char pass[] = "mihinomenestwindy";
char pathm[100];
char pathq[100];
char encodedm[20][100];
char decodedm[20][100];
char encodedq[20][100];
char decodedq[20][100];

void makeDir(char *dirName) {
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

void downloadQuote() {
    chdir(path);
    int status;
    pid_t child_id;
    child_id = fork();
    if (child_id == 0) {
        char *argv[] = {"wget", "--no-check-certificate", "https://drive.google.com/uc?id=1jR67_JAyozZPs2oYtYqEZxeBLrs-k3dt&export=download", "-O", "quote.zip", NULL};
        execv("/bin/wget", argv);
    } else {
        ((wait(&status)) > 0);
    }
}

void downloadMusic() {
    chdir(path);
    int status;
    pid_t child_id;
    child_id = fork();
    if (child_id == 0) {
        char *argv[] = {"wget", "--no-check-certificate", "https://drive.google.com/uc?id=1_djk0z-cx8bgISFsMiUaXzty2cT4VZp1&export=download", "-O", "music.zip",  NULL};
        execv("/bin/wget", argv);
    } else {
        ((wait(&status)) > 0);
    }
}

void zip() {
    chdir(path);
    int status;
    pid_t child_id;
    child_id = fork();
    if (child_id == 0) {
        char *argv[] = {"zip", "-qrP", pass, "hasil.zip", "hasil", NULL};
        execv("/usr/bin/zip", argv);
    } else {
        ((wait(&status)) > 0);
    }
}

void *processUnzip(void *tipe) {
    chdir(path);
    int status;
    pid_t child_id;
    child_id = fork();
    if (child_id == 0) {
        char *argv[] = {"unzip", "music.zip", "-d", pathm, NULL};
        execv("/usr/bin/unzip", argv);
    } else {
        ((wait(&status)) > 0);
    }

    child_id = fork();
    if (child_id == 0) {
        char *argv[] = {"unzip", "quote.zip", "-d", pathq, NULL};
        execv("/usr/bin/unzip", argv);
    } else {
        ((wait(&status)) > 0);
    }

}

// base64 decode thanks/credit to John Schambers https://nachtimwald.com/tag/base64/
const char b64chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
int b64invs[] = {62, -1, -1, -1, 63, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1, -1, -1, -1, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1, -1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51 };
size_t b64_decoded_size(const char *encd) {
    size_t len, res, i;

    if (encd == NULL) {
        return 0;
    }

    len = strlen(encd);
    res = len / 4 * 3;

    for (i = len; i-->0; ) {
        if (encd[i] == '=') {
            res--;
        } else {
            break;
        }
    }

    return res;
}

void b64_decodetable() {
    int inv[80];
    size_t i;

    memset(inv, -1, sizeof inv);
    for (i = 0; i < sizeof(b64chars) - 1; i++) {
        inv[b64chars[i] - 43] = i;
    }
}

int b64_validchar(char x) {
    int flag = 0;

    if (x >= '0' && x <= '9') {
        flag = 1;
    } else if (x >= 'A' && x <= 'Z') {
        flag = 1;
    } else if (x >= 'a' && x <= 'z') {
        flag = 1;
    } else if (x == '+' || x == '/' || x == '=') {
        flag = 1;
    }

    return flag;
}

void b64_decode(const char *encd, unsigned char *decd, size_t decdlen) {
    size_t len, i, j;
    int v;

    if (encd == NULL || decd == NULL) {
        return;
    }

    len = strlen(encd);
    if (decdlen < b64_decoded_size(encd) || len % 4 != 0) {
        return;
    }

    for (i = 0; i < len; i++) {
        if (b64_validchar(encd[i]) == 0) {
            return;
        }
    }

    for (i=0, j=0; i<len; i+=4, j+=3) {
		v = b64invs[encd[i]-43];
		v = (v << 6) | b64invs[encd[i+1]-43];
		v = encd[i+2]=='=' ? v << 6 : (v << 6) | b64invs[encd[i+2]-43];
		v = encd[i+3]=='=' ? v << 6 : (v << 6) | b64invs[encd[i+3]-43];

		decd[j] = (v >> 16) & 0xFF;
		if (encd[i+2] != '=')
			decd[j+1] = (v >> 8) & 0xFF;
		if (encd[i+3] != '=')
			decd[j+2] = v & 0xFF;
	}
}

void *moveText(void *pathl) {
    char pathh[100];
    char textType[2][10] = {"music", "quote"};
    strcpy(pathh, pathl);
    
    for (int i = 0; i < 2; i++) {
        char temp[100];
        strcpy(temp, pathh);
        if (strcmp(textType[i], "music") == 0) {
            strcat(temp, "/");
            strcat(temp, "music.txt");
        } else {
            strcat(temp, "/");
            strcat(temp, "quote.txt");
        }

        FILE *fptr;
        fptr = fopen(temp, "a+");
        // printf("%d", i);
        // printf("%s\n", textType[i]);
        if (strcmp(textType[i], "music") == 0) {
            for (int j = 0; j < 11; j++) {
                if (strlen(decodedm[j]) > 3) {
                    fprintf(fptr, "%s\n", decodedm[j]);
                    // printf("%s\n", decodedm[j]);
                    // printf("%d\n", j);
                }
            }
            // fprintf(fptr, "%s\n", decodedm);
            // printf("masuk\n");
        } else {
            for (int j = 0; j < 11; j++) {
                if (strlen(decodedq[j]) > 3) {
                    fprintf(fptr, "%s\n", decodedq[j]);
                    // printf("%s\n", decodedq[j]);
                    // printf("%d\n", j);
                }
            }
            // fprintf(fptr, "%s\n", decodedq);
            // printf("masuk\n");
        }
        fclose(fptr);
    }
}

void processText() {
    char textType[2][10] = {"music", "quote"};

    struct dirent *dp;
    DIR *dir;

    for (int i = 0; i < 2; i++) {
        char patht[100];
        if (strcmp(textType[i], "music") == 0) {
            strcpy(patht, pathm);
        } else {
            strcpy(patht, pathq);
        }

        dir = opendir(patht);
        if (!dir) {
            break;
        }

        int data = 0;
        while ((dp = readdir(dir)) != NULL) {
            if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0) {
                char temp[100];
                char *temp1;
                char *temp2;
                size_t declen;
                strcpy(temp, patht);
                strcat(temp, "/");
                strcat(temp, dp->d_name);
                // printf("%s\n", temp);
                FILE *fptr;
                fptr = fopen(temp, "a+");
                if (strcmp(textType[i], "music") == 0) {
                    fscanf(fptr, "%s", encodedm[data]);
                    // printf("%s 1\n", encodedm[data]);
                    temp1 = encodedm[data];
                    // printf("%s 2\n", encodedm[data]);
                    declen = b64_decoded_size(temp1);
                    // printf("%s 3\n", encodedm[data]);
                    temp2 = malloc(declen);
                    // printf("%s 4\n", encodedm[data]);
                    b64_decode(temp1, (unsigned char *)temp2, declen);
                    // printf("%s 5\n", encodedm[data]);
                    temp2[declen] = '\0';
                    // printf("%s 6\n", encodedm[data]);
                    strcpy(decodedm[data], temp2);
                    // printf("%s 7\n", encodedm[data]);
                    free (temp2);
                    // printf("\nmusic\n");
                    // printf("%s\n", decodedm[data]);
                } else {
                    fscanf(fptr, "%s", encodedq[data]);
                    temp1 = encodedq[data];
                    // printf("%s 2\n", encodedq[data]);
                    declen = b64_decoded_size(temp1);
                    // printf("%s 3\n", encodedq[data]);
                    temp2 = malloc(declen);
                    // printf("%s 4\n", encodedq[data]);
                    b64_decode(temp1, (unsigned char *)temp2, declen);
                    // printf("%s 5\n", encodedq[data]);
                    temp2[declen] = '\0';
                    // printf("%s 6\n", encodedq[data]);
                    strcpy(decodedq[data], temp2);
                    // printf("%s 7\n", encodedq[data]);
                    free (temp2);
                    // printf("\nquote\n");
                    // printf("%s\n", decodedq[data]);
                }
                fclose(fptr);
            }
            data++;
        }
        closedir(dir);
    }

    char pathh[100];
    strcpy(pathh, path);
    strcat(pathh, "hasil");
    makeDir(pathh);

    pthread_t tid;
    pthread_create(&(tid), NULL, moveText, (char *)pathh);
    pthread_join(tid, NULL);
}

void *forgetNo(void *arg) {
    chdir(path);
    int status;
    pid_t child_id;
    child_id = fork();
    if (child_id == 0) {
        char *argv[] = {"unzip", "-qo", "-P", pass, "hasil.zip", NULL};
        execv("/usr/bin/unzip", argv);
    } else {
        ((wait(&status)) > 0);
    }

    char ptemp[100];
    strcpy(ptemp, path);
    strcat(ptemp, "hasil/no.txt");
    FILE *fptr;
    fptr = fopen(ptemp, "a+");
    fprintf(fptr, "No");
    fclose(fptr);

    zip();

}

int main() {
    downloadMusic();
    downloadQuote();

    strcpy(pathm, path);
    strcat(pathm, "music");
    makeDir(pathm);

    strcpy(pathq, path);
    strcat(pathq, "quote");
    makeDir(pathq);

    pthread_t tid;
    pthread_create(&(tid), NULL, processUnzip, NULL);
    pthread_join(tid, NULL);

    processText();
    zip();

    pthread_t tid2;
    pthread_create(&(tid2), NULL, forgetNo, NULL);
    pthread_join(tid2, NULL);
}