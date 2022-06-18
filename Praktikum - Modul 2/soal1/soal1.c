//revisi tugas 1c
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <syslog.h>
#include <string.h>
#include <dirent.h>
#include <time.h>
#include <json-c/json.h>

// static void skeleton_daemon()
// {
//     pid_t pid;
    
//     /* Fork off the parent process */
//     pid = fork();
    
//     /* An error occurred */
//     if (pid < 0)
//         exit(EXIT_FAILURE);
    
//      /* Success: Let the parent terminate */
//     if (pid > 0)
//         exit(EXIT_SUCCESS);
    
//     /* On success: The child process becomes session leader */
//     if (setsid() < 0)
//         exit(EXIT_FAILURE);
    
//     /* Catch, ignore and handle signals */
//     /*TODO: Implement a working signal handler */
//     signal(SIGCHLD, SIG_IGN);
//     signal(SIGHUP, SIG_IGN);
    
//     /* Fork off for the second time*/
//     pid = fork();
    
//     /* An error occurred */
//     if (pid < 0)
//         exit(EXIT_FAILURE);
    
//     /* Success: Let the parent terminate */
//     if (pid > 0)
//         exit(EXIT_SUCCESS);
    
//     /* Set new file permissions */
//     umask(0);
    
//     /* Change the working directory to the root directory */
//     /* or another appropriated directory */
//     chdir("/");
    
//     /* Close all open file descriptors */
//     int x;
//     for (x = sysconf(_SC_OPEN_MAX); x>=0; x--)
//     {
//         close (x);
//     }

// }

void garputunggu(char bash[], char *arg[]){
    int status;
    pid_t child;
    child = fork();
    if(child == 0){
        execv(bash, arg);
    }
    else{
        ((wait(&status))>0);
    }
}

void zip_garputunggu(char passw[], char namaz[], char namaf[]) {
    int status;
    pid_t child;
    child = fork();
    if(child == 0){
        execlp("zip", "zip", "-P", passw, "-r", namaz, namaf, NULL);
    }
    else{
        ((wait(&status))>0);
    }
}

void rm_garputunggu(char bash[], char nm_file[]) {
    int status;
    pid_t child;
    child = fork();
    if(child == 0){
        execlp(bash, bash, nm_file, NULL);
    }
    else{
        ((wait(&status))>0);
    }
}


void rm_folder_garputunggu(char bash[], char opsi[], char nm_file[]) {
    int status;
    pid_t child;
    child = fork();
    if(child == 0){
        execlp(bash, bash, opsi, nm_file, NULL);
    }
    else{
        ((wait(&status))>0);
    }
}

int main() {
    // skeleton_daemon();

    char *nama_user;
    nama_user=(char *)malloc(10*sizeof(char));
    nama_user=getlogin();

    char dirutama[100] = "/home/";
    strcat(dirutama, nama_user);
    strcat(dirutama, "/tugas/s2/satu");

    pid_t pid, sid;        // Variabel untuk menyimpan PID

    pid = fork();     // Menyimpan PID dari Child Process

    /* Keluar saat fork gagal
    * (nilai variabel pid < 0) */
    if (pid < 0) {
        exit(EXIT_FAILURE);
    }

    /* Keluar saat fork berhasil
    * (nilai variabel pid adalah PID dari child process) */
    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    umask(0);

    sid = setsid();
    if (sid < 0) {
        exit(EXIT_FAILURE);
    }

    if ((chdir(dirutama)) < 0) {
        exit(EXIT_FAILURE);
    }

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    int m = 0;
    char gacha_weaponlist[200][200];
    int n = 0;
    char gacha_characterlist[100][200];
    struct dirent *ep;
    srand(time(0));

    int primogems = 79000;

    while(1) {
        int detik, menit, jam, hari, bulan;
        time_t now;
        time(&now);
        struct tm *local = localtime(&now);
        jam = local->tm_hour;
        menit = local->tm_min;
        detik = local->tm_sec;
        hari = local->tm_mday;
        bulan = local->tm_mon;

        if(bulan == 3 && hari == 30 && jam == 4 && menit == 44) {

            DIR *filecharacter = opendir("characters");
            if(filecharacter) {
                closedir(filecharacter);
            } else if(ENOENT == errno) {
                char link1[100] = "https://drive.google.com/uc?id=1xYYmsslb-9s8-4BDvosym7R4EmPi6BHp&export=download";

                char *chararcter[] = {"wget", "--no-check-certificate", link1,"-O","characterDB.zip", NULL};
                garputunggu("/usr/bin/wget", chararcter);
                //sleep(10);
                char *unzipchar[] =  {"unzip", "characterDB.zip", NULL};
                garputunggu("/usr/bin/unzip", unzipchar);
                
            }

            // file weapon
            DIR *fileweapon = opendir("weapons");
            if(fileweapon) {
                closedir(fileweapon);
            } else if(ENOENT == errno) {
                char link2[100] = "https://drive.google.com/uc?id=1XSkAqqjkNmzZ0AdIZQt_eWGOZ0eJyNlT&export=download";

                char *weapon[] = {"wget", "--no-check-certificate", link2,"-O","weaponDB.zip", NULL};
                garputunggu("/usr/bin/wget", weapon);
                //sleep(10);
                char *unzipweap[] =  {"unzip", "weaponDB.zip", NULL};
                garputunggu("/usr/bin/unzip", unzipweap);
                
            }

            // buat dir gacha_gacha
            DIR *filegacha = opendir("gacha_gacha");
            if(filegacha) {
                closedir(filegacha);
            } else if(ENOENT == errno) {
                char *buatdirgacha[] = {"mkdir", "gacha_gacha", NULL};
                garputunggu("/bin/mkdir", buatdirgacha);
            }



            if( m < 1 ) {
                DIR *bacaweapon = opendir("weapons");
                if(bacaweapon != NULL) {
                    while(ep = readdir(bacaweapon)) {
                        if (strcmp(ep->d_name, ".") != 0 && strcmp(ep->d_name, "..") != 0 && strstr(ep->d_name, "json")) {
                            strcpy(gacha_weaponlist[m], ep->d_name);
                            m++;
                        }        
                    } 
                    // printf("%d\n", m);
                    closedir(bacaweapon);
                } else perror ("Couldn't open the directory");
            } 

            if( n < 1 ) {
                DIR *bacacharacter = opendir("characters");
                if(bacacharacter != NULL) {
                    while(ep = readdir(bacacharacter)) {
                        if (strcmp(ep->d_name, ".") != 0 && strcmp(ep->d_name, "..") != 0) {
                            strcpy(gacha_characterlist[n], ep->d_name);
                            n++;
                        }        
                    } 
                    closedir(bacacharacter);
                } else perror ("Couldn't open the directory");
            }

            // json weapon
            // int weapon_random = rand() % m;
            // char dirawal_w[100];
            // strcpy(dirawal_w, "./weapons/");
            // strcat(dirawal_w, gacha_weaponlist[weapon_random]);
            // // printf("%s\n", dirawal_w);
            // FILE *json_weapon_file = fopen(dirawal_w, "r");
            // char isijson_weapon[3000];
            // fread(isijson_weapon, 3000, 1, json_weapon_file);
            // fclose(json_weapon_file);

            // struct json_object *parsed_json_w;
            // struct json_object *name_weapon;
            // struct json_object *rarity_weapon;

            // parsed_json_w = json_tokener_parse(isijson_weapon);

            // json_object_object_get_ex(parsed_json_w, "name", &name_weapon);
            // json_object_object_get_ex(parsed_json_w, "rarity", &rarity_weapon);
            // printf("W: %s, %s\n", json_object_get_string(name_weapon), json_object_get_string(rarity_weapon));

            //json char
            // int character_random = rand() % n;
            // char dirawal_c[100];
            // strcpy(dirawal_c, "./characters/");
            // strcat(dirawal_c, gacha_characterlist[character_random]);

            // FILE *json_character_file = fopen(dirawal_c, "r");
            // char isijson_character[3000];
            // fread(isijson_character, 3000, 1, json_character_file);
            // fclose(json_character_file);

            // struct json_object *parsed_json_c;
            // struct json_object *name_character;
            // struct json_object *rarity_character;

            // parsed_json_c = json_tokener_parse(isijson_character);

            // json_object_object_get_ex(parsed_json_c, "name", &name_character);
            // json_object_object_get_ex(parsed_json_c, "rarity", &rarity_character);
        
            // printf("C: %s, %s\n", json_object_get_string(name_character), json_object_get_string(rarity_character));
            
            int random_pilih = (rand() % 30) * 10;
            int percobaan = random_pilih;
            if(random_pilih % 90 == 0 && random_pilih != 0 && primogems > 159) {
                char sekarang[100] = "";
                int d_s = detik;
                sprintf(sekarang, "./gacha_gacha/total_gacha_%d_%d", random_pilih, d_s);
                // printf("%s", sekarang);
                char *buatdir90[] = {"mkdir", sekarang, NULL};
                garputunggu("/bin/mkdir", buatdir90);
                int menghitung = 10;

                for(int p = 0; p < percobaan/ 10 && primogems > 159; p++) {
                    int detik, menit, jam;
                    time_t now;
                    time(&now);
                    struct tm *local = localtime(&now);
                    jam = local->tm_hour;
                    menit = local->tm_min;
                    detik = local->tm_sec;
                    sleep(1);
                    strcpy(sekarang, "");
                    sprintf(sekarang, "./gacha_gacha/total_gacha_%d_%d/%d:%d:%d_gacha_%d.txt", random_pilih, d_s, jam, menit, detik, menghitung);
                    menghitung += 10;
                    FILE *buat_txt = fopen(sekarang, "w");
                    // printf("%s\n", sekarang);

                    if(buat_txt == NULL) {
                        printf("Unable to create file.\n");
                        exit(EXIT_FAILURE);
                    }
                    int ulang = 10;
                    while(ulang != 0 & primogems > 159) {
                        primogems -= 160;
                        if(ulang % 2 == 0) {
                            int weapon_random = rand() % m;
                            char dirawal_w[100];
                            strcpy(dirawal_w, "./weapons/");
                            strcat(dirawal_w, gacha_weaponlist[weapon_random]);
                            // printf("%s\n", dirawal_w);
                            FILE *json_weapon_file = fopen(dirawal_w, "r");
                            char isijson_weapon[3000];
                            fread(isijson_weapon, 3000, 1, json_weapon_file);
                            fclose(json_weapon_file);

                            struct json_object *parsed_json_w;
                            struct json_object *name_weapon;
                            struct json_object *rarity_weapon;

                            parsed_json_w = json_tokener_parse(isijson_weapon);

                            json_object_object_get_ex(parsed_json_w, "name", &name_weapon);
                            json_object_object_get_ex(parsed_json_w, "rarity", &rarity_weapon);

                            fprintf(buat_txt, "%d_weapons_%s_%s_%d\n", random_pilih, json_object_get_string(rarity_weapon), json_object_get_string(name_weapon), primogems);
                            ulang -= 1;
                        } else {
                            int character_random = rand() % n;
                            char dirawal_c[100];
                            strcpy(dirawal_c, "./characters/");
                            strcat(dirawal_c, gacha_characterlist[character_random]);

                            FILE *json_character_file = fopen(dirawal_c, "r");
                            char isijson_character[3000];
                            fread(isijson_character, 3000, 1, json_character_file);
                            fclose(json_character_file);

                            struct json_object *parsed_json_c;
                            struct json_object *name_character;
                            struct json_object *rarity_character;

                            parsed_json_c = json_tokener_parse(isijson_character);

                            json_object_object_get_ex(parsed_json_c, "name", &name_character);
                            json_object_object_get_ex(parsed_json_c, "rarity", &rarity_character);

                            fprintf(buat_txt, "%d_characters_%s_%s_%d\n", random_pilih, json_object_get_string(rarity_character), json_object_get_string(name_character), primogems);
                            ulang -= 1;
                        }
                    }
                    fclose(buat_txt);
                }
    
            } else if (random_pilih % 10 == 0 & random_pilih != 0 && primogems > 159) {
                char waktu[100] = "";
                sprintf(waktu, "./gacha_gacha/%d:%d:%d_gacha_%d.txt", jam, menit, detik, random_pilih);
                // printf("%s", waktu);
                FILE *buat_txt = fopen(waktu, "w");

                if(buat_txt == NULL) {
                    printf("Unable to create file.\n");
                    exit(EXIT_FAILURE);
                }
                while(percobaan != 0 && primogems > 159) {
                    if (primogems / 160 > 0) {
                        if(percobaan % 2 == 0) {
                            int weapon_random = rand() % m;
                            char dirawal_w[100];
                            strcpy(dirawal_w, "./weapons/");
                            strcat(dirawal_w, gacha_weaponlist[weapon_random]);
                            // printf("%s\n", dirawal_w);
                            FILE *json_weapon_file = fopen(dirawal_w, "r");
                            char isijson_weapon[3000];
                            fread(isijson_weapon, 3000, 1, json_weapon_file);
                            fclose(json_weapon_file);

                            struct json_object *parsed_json_w;
                            struct json_object *name_weapon;
                            struct json_object *rarity_weapon;

                            parsed_json_w = json_tokener_parse(isijson_weapon);

                            json_object_object_get_ex(parsed_json_w, "name", &name_weapon);
                            json_object_object_get_ex(parsed_json_w, "rarity", &rarity_weapon);

                            primogems -= 160;
                            fprintf(buat_txt, "%d_weapons_%s_%s_%d\n", random_pilih, json_object_get_string(rarity_weapon), json_object_get_string(name_weapon), primogems);
                            percobaan -= 1;
                        } else {
                            int character_random = rand() % n;
                            char dirawal_c[100];
                            strcpy(dirawal_c, "./characters/");
                            strcat(dirawal_c, gacha_characterlist[character_random]);

                            FILE *json_character_file = fopen(dirawal_c, "r");
                            char isijson_character[3000];
                            fread(isijson_character, 3000, 1, json_character_file);
                            fclose(json_character_file);

                            struct json_object *parsed_json_c;
                            struct json_object *name_character;
                            struct json_object *rarity_character;

                            parsed_json_c = json_tokener_parse(isijson_character);

                            json_object_object_get_ex(parsed_json_c, "name", &name_character);
                            json_object_object_get_ex(parsed_json_c, "rarity", &rarity_character);

                            primogems -= 160;
                            fprintf(buat_txt, "%d_characters_%s_%s_%d\n", random_pilih, json_object_get_string(rarity_character), json_object_get_string(name_character), primogems);
                            percobaan -= 1;
                        }
                    } else {
                        fprintf(buat_txt, "Primogems anda habis");
                        percobaan = 0;
                        // return EXIT_SUCCESS;
                    }
                    
                }
                fclose(buat_txt);


            }
            


        }
        else if(bulan == 3 && hari == 30 && jam == 7 && menit == 44) {
            zip_garputunggu("satuduatiga", "not_safe_for_wibu.zip", "gacha_gacha");
            rm_folder_garputunggu("rm", "-r", "./gacha_gacha");
            rm_folder_garputunggu("rm", "-r", "./weapons");
            rm_folder_garputunggu("rm", "-r", "./characters");
            rm_garputunggu("rm", "characterDB.zip");
            rm_garputunggu("rm", "weaponDB.zip");
            return EXIT_SUCCESS;
        }   
        
        sleep(1);
    }
    

    // return EXIT_SUCCESS;
    
}
