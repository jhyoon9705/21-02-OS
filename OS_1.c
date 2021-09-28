#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1

void type_prompt(){
    printf("Jinho-Yoon$ ");
}

void read_command ( char command[], char *params[]) {
    char com[100] = {0};
    int counter = 0, i = 0;
    char *arr[100], *pointer;

    while (TRUE) {
        char c = fgetc(stdin);
        com[counter++] = c;
        if(c=='\n') 
	    break;
    }

     pointer = strtok(com, " \n");

    while (pointer != NULL) {
        arr[i++] = pointer;
	pointer = strtok(NULL, " \n");
    }

    strcpy(command, arr[0]);

    for(int j=0; j<i; j++)
        params[j] = arr[j];

    params[i]=NULL;
}



int main() {
    char cmd[100];
    char command[100], *ps_argv[100];
    char *const ps_envp[] = {"PATH=/bin", 0};

    while (TRUE) {

        type_prompt();
	read_command(command, ps_argv);

	if (strcmp(command, "exit")==0)
            break;

        if(fork()!=0) {
	/* Parent Code */
            wait(NULL);
    }
        else {
	/* Child Code */
	    strcpy(cmd, "/bin/"); 
	    strcat(cmd, command);
	    execve(cmd, ps_argv, ps_envp);
	    break;
        }
       
    }
    return 0;
} 
