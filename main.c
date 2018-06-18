#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef int bool;
enum {false,true};  //false=0;true=1

void displayShell_Id(char *userName,char*shellName)
{
    strcat(userName," : $");
    char *shell_Id=(char*)malloc(strlen(userName)+strlen(shellName));

    strcat(shell_Id,shellName); strcat(shell_Id,userName);
    printf("%s",shell_Id);
}

int main()
{
    //declear & init vars
    char *userName=getlogin();
    char *shellName="Commander_";

    displayShell_Id(userName,shellName);

    return 0;
}
