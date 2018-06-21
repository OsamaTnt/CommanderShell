#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

typedef int bool;
enum {false,true};  //false=0;true=1

const int MAX_ARGS_SIZE =15;

size_t i,j;

void displayShell_Id(char *userName,char*shellName)
{
    strcat(userName,": $");
    char *shell_Id=(char*)malloc(strlen(userName)+strlen(shellName));

    strcat(shell_Id,shellName); strcat(shell_Id,userName);
    printf("%s",shell_Id);
}

char *getCommandLine()
{
    char *S=NULL;
    size_t MAX_SIZE=0;
    getline(&S,&MAX_SIZE,stdin);

    //get rid of '\n' by replacing it with '\0'
    for(i=0;i<strlen(S);i++) { if(S[i]=='\n'){S[i]='\0';break;} }
    return S;
}

void display_Args(char *commandArgs[])
{
    i=0;while(commandArgs[i]!=NULL) {printf("%s\n",commandArgs[i]);i++;}
}

void setArgs(char *userCommand,char *commandArgs[])
{
    for(i=0;i<(int)strlen(userCommand);i++)
    {
        commandArgs[i] = malloc(sizeof(char*));
        if(i==0) {commandArgs[i] = strtok(userCommand," \n");}
        else
        {
            if(commandArgs[i]!=NULL){commandArgs[i] = strtok(NULL," \n");} else {break;}
        }
    }
}

int main(int argc,char *argv[])
{

    char *userName=getlogin();
    char *shellName="Commander_";

    int *status=NULL;

    bool bExit=false;

    pid_t proc_Id;

    char *userCommand=NULL;
    char *commandArgs[MAX_ARGS_SIZE];
    for(i=0;i<MAX_ARGS_SIZE;i++) {commandArgs[i]=NULL;}

    while(!bExit)
    {
        displayShell_Id(userName,shellName);
        userCommand = getCommandLine();
        if(userCommand) {setArgs(userCommand,commandArgs);}
        //DisplayArgs(commandArgs);

        proc_Id=fork();

        if(strcmp(userCommand,"exit")==0) {_exit(0);}

        if(proc_Id==0)
        { execvp(argv[0],argv); }

        else if(waitpid(proc_Id,status,0)==proc_Id) {_exit(0);}
        else {printf("Err\n");_exit(0);}
    }

    return 0;
}
