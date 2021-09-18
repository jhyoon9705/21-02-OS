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
    char com[50] = {0};
    int counter = 0, i = 0;
    char *arr[50], *pointer;

    while (TRUE) {
        char c = fgetc(stdin);
        com[counter++] = c;
        if(c=='\n')
        break;
    }

    if (counter == 1)
        return;
    
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
    char cmd[50];
    char command[50], *parameters[50];
    char *const ps_envp[] = {(char*) "PATH=/bin", 0};

    while (TRUE) {

        type_prompt();
    read_command(command, parameters);

    if (strcmp(command, "exit")==0)
            break;

        if(fork()!=0)
            wait(NULL);
        else {
        strcpy(cmd, "/bin/");
        strcat(cmd, command);
        execve(cmd, parameters, ps_envp);
       
        }
       
    }
    return 0;
}  
