#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>

int main() {
	pid_t childID;
	int status;

  	childID = fork();

  	if (childID < 0) {
    		exit(EXIT_FAILURE); // program berhenti jika gagal membuat program baru
  	}

  	if (childID == 0) { // child
    		char *argv[] = {"mkdir", "-p", "modul2/darat/", NULL};
    		execv("/bin/mkdir", argv);
  	} 
  
  	else { // parent
    		while ((wait(&status)) > 0);
    		char *argv[] = {"mkdir", "-p", "modul2/air/", NULL};
   		execv("/usr/bin/mkdir", argv);
   		sleep(3);
  	}
}

