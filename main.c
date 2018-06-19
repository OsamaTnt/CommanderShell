#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


typedef int bool;
enum {false,true};  //false=0;true=1

void displayShell_Id(char *userName,char*shellName)
{
    strcat(userName,": $");
    char *shell_Id=(char*)malloc(strlen(userName)+strlen(shellName));

    strcat(shell_Id,shellName); strcat(shell_Id,userName);
    printf("%s",shell_Id);
}

char *getCommandLine()
{
    char *S=malloc(sizeof(char*));
    size_t MAX_SIZE=0,i=0;

    getline(&S,&MAX_SIZE,stdin);

    //get rid of '\n' by replacing it with '\0'
    for(i=0;i<strlen(S);i++) { if(S[i]=='\n'){S[i]='\0';break;} }

    return S;
}

int main(int argc,char *argv[])
{

    char *userName=getlogin();
    char *shellName="Commander_";

    int *status=NULL;

    bool bExit=false;

    pid_t proc_Id;

    char *userCommand=NULL;

    while(!bExit)
    {
        displayShell_Id(userName,shellName);
        userCommand = getCommandLine();

        proc_Id=fork();

        if(strcmp(userCommand,"exit")==0) {_exit(0);}

        if(proc_Id==0)
        {
            execvp(argv[0],argv);
        }

        else if(waitpid(proc_Id,status,0)==proc_Id) {_exit(0);}

        else {printf("Err\n");_exit(0);}

    }

    return 0;
}
