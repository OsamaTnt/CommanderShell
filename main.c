#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "ANSI_COLORS.h"

typedef int bool;
enum {false,true};  //false=0;true=1

enum { Err_ChangeTextColor,Err_COLOR_NAME,
       Err_Create,Err_NOT_SUPPORTED_TYPE,Err_NO_SPECIFIED_PATH_NAME,Err_COULD_NOT_CREAT };

const int MAX_ARGS_SIZE =15;
size_t i,j;

void displayShell_Id(char *userName,char*shellName)
{
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
            if(commandArgs[i]!=NULL){commandArgs[i] = strtok(NULL," \n");}
            else {break;}
        }
    }
}

/* This function will return a lowerCase copy of the original string */
char* getLower(char *S)
{
    i=0;  char *temp=(char*)malloc(strlen(S)+1);
    while(i<strlen(S))
    {
        if(S[i]>=65 && S[i]<=90) {temp[i]=S[i]+32;}
        else {temp[i]=S[i];} i++;
    }
    return temp;
}

void Err_Manager(int Err_ID)
{
    switch(Err_ID)
    {
        /*00.$changeTextColor -[COLOR_NAME]*/
        case Err_ChangeTextColor :
            {printf("\nCOMMAND_ERROR :\n\t -Try $changeTextColor -[COLOR_NAME]\n\n"); break;}
        case Err_COLOR_NAME :
            {printf("\nCOMMAND_ERROR : _NOT_SUPPORTED_COLOR_CODE\n\n\t-Try $changeTextColor [COLOR_NAME]\n\n"); break;}


        /*01.$Create -[TYPE] -[PATH/TO/SPECIFIED-NAME]*/
        case Err_Create : printf("Err_Create\n");
            break;
        case Err_NOT_SUPPORTED_TYPE : printf("Err_NOT_SUPPORTED_TYPE\n");
            break;
        case Err_NO_SPECIFIED_PATH_NAME : printf("Err_NO_SPECIFIED_PATH_NAME\n");
            break;
        case Err_COULD_NOT_CREAT : printf("Err_COULD_NOT_CREAT\n");
            break;

        /*Defual*/
        default :
            break;

    }
}

/*
    changeTextColor() "How it Works" :
    if ( COLOR_NAME IS CORRECT && COLOR_IS_SUPPORTED) {UPDATE_COLOR && return(0)}
    else {Err && return(-1)}
*/
int changeTextColor(char *COLOR_NAME)
{
    if( (strcmp(getLower(COLOR_NAME),"-black")==0) || (strcmp(getLower(COLOR_NAME),"black")==0) )
     {printf("%s\n",ANSI_BLACK); return 0;}
    else if( (strcmp(getLower(COLOR_NAME),"-red")==0) || (strcmp(getLower(COLOR_NAME),"red")==0) )
     {printf("%s\n",ANSI_RED); return 0;}
    else if( (strcmp(getLower(COLOR_NAME),"-green")==0) || (strcmp(getLower(COLOR_NAME),"green")==0) )
     {printf("%s\n",ANSI_GREEN); return 0;}
    else if( (strcmp(getLower(COLOR_NAME),"-brown")==0) || (strcmp(getLower(COLOR_NAME),"brown")==0) )
     {printf("%s\n",ANSI_BROWN); return 0;}
    else if( (strcmp(getLower(COLOR_NAME),"-blue")==0) || (strcmp(getLower(COLOR_NAME),"blue")==0) )
     {printf("%s\n",ANSI_BLUE); return 0;}
    else if( (strcmp(getLower(COLOR_NAME),"-magenta")==0) || (strcmp(getLower(COLOR_NAME),"magenta")==0) )
     {printf("%s\n",ANSI_MAGENTA); return 0;}
    else if( (strcmp(getLower(COLOR_NAME),"-cyan")==0) || (strcmp(getLower(COLOR_NAME),"cyan")==0) )
     {printf("%s\n",ANSI_CYAN); return 0;}
    else if( (strcmp(getLower(COLOR_NAME),"-gray")==0) || (strcmp(getLower(COLOR_NAME),"gray")==0) )
     {printf("%s\n",ANSI_GRAY); return 0;}
    else if( (strcmp(getLower(COLOR_NAME),"-yellow")==0) || (strcmp(getLower(COLOR_NAME),"yellow")==0) )
     {printf("%s\n",ANSI_YELLOW); return 0;}
    else if( (strcmp(getLower(COLOR_NAME),"-white")==0) || (strcmp(getLower(COLOR_NAME),"white")==0) )
     {printf("%s\n",ANSI_WHITE); return 0;}
    else if( (strcmp(getLower(COLOR_NAME),"-dark_gray")==0) || (strcmp(getLower(COLOR_NAME),"dark_gray")==0) )
     {printf("%s\n",ANSI_DARK_GRAY); return 0;}
    else if( (strcmp(getLower(COLOR_NAME),"-light_red")==0) || (strcmp(getLower(COLOR_NAME),"light_red")==0) )
     {printf("%s\n",ANSI_LIGHT_RED); return 0;}
    else if( (strcmp(getLower(COLOR_NAME),"-light_green")==0) || (strcmp(getLower(COLOR_NAME),"light_green")==0) )
     {printf("%s\n",ANSI_LIGHT_GREEN); return 0;}
    else if( (strcmp(getLower(COLOR_NAME),"-light_blue")==0) || (strcmp(getLower(COLOR_NAME),"light_blue")==0) )
     {printf("%s\n",ANSI_LIGHT_BLUE); return 0;}
    else if( (strcmp(getLower(COLOR_NAME),"-light_magenta")==0) || (strcmp(getLower(COLOR_NAME),"light_magenta")==0) )
     {printf("%s\n",ANSI_LIGHT_MAGENTA); return 0;}
    else if( (strcmp(getLower(COLOR_NAME),"-light_cyan")==0) || (strcmp(getLower(COLOR_NAME),"light_cyan")==0) )
     {printf("%s\n",ANSI_LIGHT_CYAN); return 0;}
    else {return -1;}
}

int Create(char *Type,char *Name)
{
    return -1;
}

void proc_Commands(char *commandArgs[])
{
    if(commandArgs[0])
    {
        /*00. proc %changeTextColor -[COLOR_NAME]*/
        if(strcmp(getLower(commandArgs[0]),"changetextcolor")==0)
        {
            if(commandArgs[1])
            {
                if(changeTextColor(commandArgs[1])==-1) {Err_Manager(Err_COLOR_NAME);}
            } else {Err_Manager(Err_ChangeTextColor);}
        }

        /*01. proc $Create -[TYPE] -[PATH/TO/SPECIFIED-NAME]*/
        else if(strcmp(getLower(commandArgs[0]),"create")==0)
        {
            if(commandArgs[1])
            {
                if( (strcmp(getLower(commandArgs[1]),"-d")==0) || (strcmp(getLower(commandArgs[1]),"-f")==0)
                  || (strcmp(getLower(commandArgs[1]),"d")==0) || (strcmp(getLower(commandArgs[1]),"f")==0) )
                {
                    if(commandArgs[2])
                    {
                        if(Create(commandArgs[1],commandArgs[2])==-1) {Err_Manager(Err_COULD_NOT_CREAT);}
                    } else {Err_Manager(Err_NO_SPECIFIED_PATH_NAME);}

                } else
                  {
                     Err_Manager(Err_NOT_SUPPORTED_TYPE);
                     if(!commandArgs[2]) {Err_Manager(Err_NO_SPECIFIED_PATH_NAME);}
                  }

            } else {Err_Manager(Err_Create);}
        }
    }
}

int main()
{

    char *userName=getlogin(); strcat(userName,": $");
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

        //check if $exit
        if(strcmp(getLower(userCommand),"exit")==0) {_exit(0);}

        if(proc_Id==0)
        {
            execvp(commandArgs[0],commandArgs);
            proc_Commands(commandArgs);
            _exit(0);
        }

        else if(waitpid(proc_Id,status,0)==proc_Id) {}
        else {printf("Err\n");_exit(0);}
    }
    return 0;
}
