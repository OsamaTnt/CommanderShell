#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <dirent.h>
#include <time.h>
#include "ANSI_COLORS.h"

typedef int bool;
enum {false,true};  //false=0;true=1

enum { Err_Usage,Err_NOT_EXISTS,Err_ALREADY_EXISTS,Err_NO_SPECIFIED_NEW_NAME,Err_COULD_NOT,
       Err_COLOR_NAME,
       Err_NOT_SUPPORTED_TYPE,Err_NO_SPECIFIED_PATH_TO_NAME};

enum {CHANGE_TEXT_COLOR,CREATE,DELETE,CHANGE_NAME,MAKE_COPY,ENCRYPT,DECRYPT};

const int MAX_ARGS_SIZE =15;
const int MAX_PATH_LENGTH=512;
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

void Err_Manager(int Err_ID,int Command_ID)
{
    switch(Err_ID)
    {
        /*General Errs*/
        case Err_Usage :
        {
            if(Command_ID==CHANGE_TEXT_COLOR)
             {printf("\nCOMMAND_ERROR :\n\t -Try $changeTextColor -[COLOR_NAME]\n\n");}
            else if(Command_ID==CREATE)
             {printf("\nCOMMAND_ERROR :\n\t -Try $Create -[TYPE] -[PATH/TO/NEW-NAME]\n\n");}
            else if(Command_ID==DELETE)
             {printf("\nCOMMAND_ERROR :\n\t -Try $Delete -[TYPE] -[PATH/TO/SPECIFIED-NAME]\n\n");}
            else if(Command_ID==CHANGE_NAME)
             {printf("\nCOMMAND_ERROR :\n\t -Try $changeName -[PATH/TO/OLD-NAME] -[NEW-NAME]\n\n");}
            else if(Command_ID==MAKE_COPY)
             {printf("\nCOMMAND_ERROR :\n\t -Try $makeCopy -[PATH/TO/SRC_NAME] -[PATH/TO/NEW_COPY]\n\n");}
            else if(Command_ID==ENCRYPT)
             {printf("\nCOMMAND_ERROR :\n\t -Try $Encrypt -[PATH/TO/SRC_NAME]\n\n");}
            else if(Command_ID==DECRYPT)
             {printf("\nCOMMAND_ERROR :\n\t -Try $Decrypt -[PATH/TO/SRC_NAME]\n\n");}
            break;
        }

        case Err_NOT_EXISTS :
        {
            if(Command_ID==DELETE)
             {printf("\nCOMMAND_ERROR : _NOT_EXISTS \n\n\t-Try $Delete -[TYPE] -[PATH/TO/SPECIFIED-NAME]\n\n");}
            else if(Command_ID==CHANGE_NAME)
             {printf("\nCOMMAND_ERROR : _NOT_EXISTS \n\n\t-Try $changeNames -[PATH/TO/OLD-NAME] -[NEW-NAME]\n\n");}
            else if(Command_ID==MAKE_COPY)
             {printf("\nCOMMAND_ERROR : _NOT_EXISTS \n\n\t-Try $makeCopy -[PATH/TO/SRC_NAME] -[PATH/TO/NEW_COPY]\n\n");}
            else if(Command_ID==ENCRYPT)
             {printf("\nCOMMAND_ERROR : _NOT_EXISTS \n\n\t-Try $Encrypt -[PATH/TO/SRC_NAME]\n\n");}
            break;
        }

        case Err_ALREADY_EXISTS :
        {
            if(Command_ID==CREATE)
             {printf("\nCOMMAND_ERROR : _ALREADY_EXISTS \n\n\t-Try $Create -[TYPE] -[PATH/TO/NEW-NAME]\n\n");}
            else if(Command_ID==MAKE_COPY)
             {printf("\nCOMMAND_ERROR : _ALREADY_EXISTS \n\n\t-Try $makeCopy -[PATH/TO/SRC_NAME] -[PATH/TO/NEW_COPY]\n\n");}
            break;
        }

        case Err_NO_SPECIFIED_NEW_NAME :
        {
            if(Command_ID==CHANGE_NAME)
             {printf("\nCOMMAND_ERROR : _NO_SPECIFIED_NEW_NAME \n\n\t-Try $changeName -[PATH/TO/OLD-NAME] -[NEW-NAME]\n\n");}
            else if(Command_ID==MAKE_COPY)
             {printf("\nCOMMAND_ERROR : _NO_SPECIFIED_NEW_NAME \n\n\t-Try $makeCopy -[PATH/TO/SRC_NAME] -[PATH/TO/NEW_COPY]\n\n");}
            break;
        }

        case Err_COULD_NOT :
        {
            if(Command_ID==CREATE)
             {printf("\nCOMMAND_ERROR : _COULD_NOT_CREATE \n\n\t-Try $Create -[TYPE] -[PATH/TO/NEW-NAME]\n\n");}
            else if(Command_ID==DELETE)
             {printf("\nCOMMAND_ERROR : _COULD_NOT_DELETE \n\n\t-Try $Delete -[TYPE] -[PATH/TO/SPECIFIED-NAME]\n\n");}
            else if(Command_ID==CHANGE_NAME)
             {printf("\nCOMMAND_ERROR : _COULD_NOT_RENAME \n\n\t-Try $changeNames -[PATH/TO/OLD-NAME] -[NEW-NAME]\n\n");}
            else if(Command_ID==MAKE_COPY)
             {printf("\nCOMMAND_ERROR : _COULD_NOT_COPY \n\n\t-Try $makeCopy -[PATH/TO/SRC_NAME] -[PATH/TO/NEW_COPY]\n\n");}
            else if(Command_ID==ENCRYPT)
             {printf("\nCOMMAND_ERROR : _COULD_NOT_ENCRYPT \n\n\t-Try $Encrypt -[PATH/TO/SRC_NAME]\n\n");}
            else if(Command_ID==DECRYPT)
             {printf("\nCOMMAND_ERROR : _COULD_NOT_DECRYPT \n\n\t-Try $Decrypt -[PATH/TO/SRC_NAME]\n\n");}
            break;
        }


        /*$changeTextColor -[COLOR_NAME] local Errs*/
        case Err_COLOR_NAME :
            {printf("\nCOMMAND_ERROR : _NOT_SUPPORTED_COLOR_CODE\n\n\t-Try $changeTextColor [COLOR_NAME]\n\n"); break;}


        /*$Create -[TYPE] -[PATH/TO/SPECIFIED-NAME] local Errs*/
        case Err_NOT_SUPPORTED_TYPE :
            {printf("\nCOMMAND_ERROR : _NOT_SUPPORTED_TYPE \n\n\t-Try $Create -[TYPE] -[PATH/TO/NEW-NAME]\n\n"); break;}
        case Err_NO_SPECIFIED_PATH_TO_NAME :
            {printf("\nCOMMAND_ERROR : _NO_SPECIFIED_PATH_TO_NAME \n\n\t-Try $Create -[TYPE] -[PATH/TO/NEW-NAME]\n\n"); break;}


        /*Defual*/
        default :
            break;

    }
}

/*
    changeTextColor() ".How it Works." :
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

bool bIsFileExists(char *PATH_TO_NAME)
{
    if(fopen(PATH_TO_NAME,"r")) {return true;}
    else {return false;}
}

bool bIsDirExists(char *PATH_TO_NAME)
{
    if(opendir(PATH_TO_NAME)) {return true;}
    else {return false;}
}

int Create(char *Type,char *PATH_TO_NAME)
{
    //if Type was a File
    if( (strcmp(getLower(Type),"-f")==0) || (strcmp(getLower(Type),"f")==0) )
    {
        if(bIsFileExists(PATH_TO_NAME))
        {Err_Manager(Err_ALREADY_EXISTS,CREATE); return 0;}

        else
        {
            FILE *F=fopen(PATH_TO_NAME,"w");

            if(!F) {return -1;}
            else {fclose(F); return 0;}
        }
    }

    //if Type was a directory
    else if( (strcmp(getLower(Type),"-d")==0) || (strcmp(getLower(Type),"d")==0) )
    {
        if(bIsDirExists(PATH_TO_NAME))
        {Err_Manager(Err_ALREADY_EXISTS,CREATE); return 0; }

        else
        {
            if(mkdir(PATH_TO_NAME,ACCESSPERMS)==0) { return 0;}
            else {return -1;}
        }
    }

    else {return -1;}

}

int _Delete(char *Type,char *PATH_TO_NAME)
{
    //if Type was a File
    if( (strcmp(getLower(Type),"-f")==0) || (strcmp(getLower(Type),"f")==0) )
    {
        if(!bIsFileExists(PATH_TO_NAME)) { Err_Manager(Err_NOT_EXISTS,DELETE); return 0;}

        else
        {
            if(unlink(PATH_TO_NAME)==-1) {return -1;}
            else {return 0;}
        }
    }

    //if Type was a Directory
    /*
        This is not a flexible method, it can only delete an empty directory
        to be able to delete a full file tree it will be a little bit complicated,
        but it can be done by calling nftw() recursivly
    */
    else if( (strcmp(getLower(Type),"-d")==0) || (strcmp(getLower(Type),"d")==0) )
    {
        if(!bIsDirExists(PATH_TO_NAME)) { Err_Manager(Err_NOT_EXISTS,DELETE); return 0;}

        else
        {
            if(rmdir(PATH_TO_NAME)==-1) {return -1;}
            else {return 0;}
        }
    }

    else {return -1;}

}

int changeName(char *PATH_OLD_NAME,char *NEW_NAME)
{
    if(!bIsDirExists(PATH_OLD_NAME) && !bIsFileExists(PATH_OLD_NAME))
    {Err_Manager(Err_NOT_EXISTS,CHANGE_NAME); return 0;}

    else
    {
        char *PATH_NEW_NAME=NULL;
        int Pos_To_Name=0;
        bool bIsPath=false;

        for(i=0;i<strlen(PATH_OLD_NAME);i++)
        {
            if(PATH_OLD_NAME[i]=='/') {bIsPath=true; Pos_To_Name=i;}
        }

        if(!bIsPath) {PATH_NEW_NAME = NEW_NAME;}

        else
        {
            PATH_NEW_NAME = malloc(sizeof(Pos_To_Name+1));

            for(i=0;i<=Pos_To_Name;i++)
            {PATH_NEW_NAME[i] = PATH_OLD_NAME[i];}

            PATH_NEW_NAME = (char *)realloc(PATH_NEW_NAME,strlen(PATH_NEW_NAME)+strlen(NEW_NAME)+1);
            strcat(PATH_NEW_NAME,NEW_NAME);
        }

        if(rename(PATH_OLD_NAME,PATH_NEW_NAME)==-1) {return -1;}
        else {return 0;}

    }
}

/*
    The Copy will works only for Type -[f] for now
    the support for Type -[d] will be availabe on the next updates..
*/
int makeCopy(char *PATH_TO_SRC,char *PATH_TO_NEW_COPY)
{
    if(!bIsFileExists(PATH_TO_SRC) && !bIsDirExists(PATH_TO_SRC))
     {Err_Manager(Err_NOT_EXISTS,MAKE_COPY); return 0;}

    else if(PATH_TO_SRC==PATH_TO_NEW_COPY || bIsFileExists(PATH_TO_NEW_COPY) || bIsDirExists(PATH_TO_NEW_COPY))
     {Err_Manager(Err_ALREADY_EXISTS,MAKE_COPY); return 0;}

    //if Type was a File
    else if(bIsFileExists(PATH_TO_SRC) && !bIsDirExists(PATH_TO_SRC))
    {
        FILE *srcFile=fopen(PATH_TO_SRC,"r"), *desFile=fopen(PATH_TO_NEW_COPY,"w");
        char c;

        while( (c=fgetc(srcFile))!=EOF )
        {fputc(c,desFile);}

        fclose(srcFile); fclose(desFile);
        return 0;
    }

    //If Type was a Directory (Soon..)
    else if(bIsDirExists(PATH_TO_SRC) && !bIsFileExists(PATH_TO_SRC) )
    {
         /* What's 1000-7 ? */ return -1;
    }

    else { return -1; }
}

char *getLastFromPath(char *PATH_TO_NAME)
{
    char *LAST_NAME=NULL;
    int Pos_To_Name=0;
    bool bIsPath=false;

    for(i=0;i<strlen(PATH_TO_NAME);i++)
    {
        if(PATH_TO_NAME[i]=='/') {bIsPath=true; Pos_To_Name=i;}
    }

    if(!bIsPath) {LAST_NAME = PATH_TO_NAME;}
    else
    {
        LAST_NAME = malloc(strlen(PATH_TO_NAME)-Pos_To_Name);

        for(i=0,j=Pos_To_Name+1;j<=strlen(PATH_TO_NAME);i++,j++)
        {LAST_NAME[i] = PATH_TO_NAME[j];}
    }

    return LAST_NAME;
}

char *getCurrentDir()
{
    char *currentDir=(char*)malloc(MAX_PATH_LENGTH);
    getcwd(currentDir,MAX_PATH_LENGTH);
    return currentDir;
}

/*
  ".How it Works." :
    - the encryption method is simply rely on random ascii-code that will be generated for each char inside the srcFile
    - an auto generated backupFile will be created & stored inside BackupFolder on the same path (auto-created if non found)
    - the backupFile will be used later-on to decrypt the srcFile && the backupFile will be auto-removed
*/
int encrypt(char *PATH_TO_NAME)
{
    FILE *srcFile=fopen(PATH_TO_NAME,"r"), *updated_srcFile=fopen(PATH_TO_NAME,"r+");
    if(!bIsFileExists(PATH_TO_NAME) || !srcFile) { Err_Manager(Err_NOT_EXISTS,ENCRYPT); return 0; }

    else
    {
        //set the BACKUP_PATH && BACKUP_NAME
        char *BACKUP_PATH=(char*)malloc(MAX_PATH_LENGTH);
        char *BACKUP_ID="-d@";

        if(!bIsDirExists("BackupFolder")) {Create("d","BackupFolder");}

        strcat(BACKUP_PATH,getCurrentDir());  strcat(BACKUP_PATH,"/BackupFolder/");

        char *BACKUP_NAME = malloc(strlen(getLastFromPath(PATH_TO_NAME))+strlen(BACKUP_ID));
        strcat(BACKUP_NAME,BACKUP_ID);  strcat(BACKUP_NAME,getLastFromPath(PATH_TO_NAME));

        strcat(BACKUP_PATH,BACKUP_NAME);

        //create the auto generated backupFile inside BackupFolder
        FILE *backupFile = fopen(BACKUP_PATH,"w");
        if(!backupFile) {return -1;}

        //do the magic here..
        else
        {
            char c;
            int Rand_Group,Rand_ASCII_ID,Rand_Char,Rand_SpecialChar;

            //update & seed the rand() with the current time (seconds!)
            srand(time(NULL));

            while( (c=fgetc(srcFile))!=EOF )
            {
                //We don't wont the \n to get involved in encryption process
                if(c!='\n')
                {
                    Rand_Group = rand()%3;
                    switch(Rand_Group)
                    {
                        case 0: /*numerical-range*/
                         {Rand_ASCII_ID = rand()%10+48; break;}

                        case 1: /*alphabatical-range*/
                        {
                            Rand_Char = rand()%2;
                            if(Rand_Char==0) {Rand_ASCII_ID = rand()%26+65;}
                            else {Rand_ASCII_ID = rand()%26+97;} break;
                        }

                        case 2: /*specialChar-range*/
                        {
                            Rand_SpecialChar = rand()%3;
                            if(Rand_SpecialChar==0) {Rand_ASCII_ID = rand()%16+32;}
                            else if(Rand_SpecialChar==1) {Rand_ASCII_ID = rand()%7+58;}
                            else {Rand_ASCII_ID = rand()%4+123;} break;
                        }

                        default: {break;}
                    }
                    fputc(c,backupFile);
                    fputc(Rand_ASCII_ID,updated_srcFile);
                }

                /*[OPTINAL] save '\n' from encryption*/
                else { fputc('\n',updated_srcFile); fputc('\n',backupFile); }

            }//END_OF_LOOP

            fclose(srcFile); fclose(backupFile); fclose(updated_srcFile);
        }

        return 0;
    }
}

int _Decrypt(char *PATH_TO_NAME)
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
                if(changeTextColor(commandArgs[1])==-1) {Err_Manager(Err_COLOR_NAME,CHANGE_TEXT_COLOR);}
            } else {Err_Manager(Err_Usage,CHANGE_TEXT_COLOR);}
        }

        /*01. proc $Create -[TYPE] -[PATH/TO/NEW-NAME]*/
        else if(strcmp(getLower(commandArgs[0]),"create")==0)
        {
            if(commandArgs[1])
            {
                if( (strcmp(getLower(commandArgs[1]),"-d")==0) || (strcmp(getLower(commandArgs[1]),"-f")==0)
                  || (strcmp(getLower(commandArgs[1]),"d")==0) || (strcmp(getLower(commandArgs[1]),"f")==0) )
                {
                    if(commandArgs[2])
                    {
                        if(Create(commandArgs[1],commandArgs[2])==-1) {Err_Manager(Err_COULD_NOT,CREATE);}
                    } else {Err_Manager(Err_NO_SPECIFIED_PATH_TO_NAME,CREATE);}

                } else
                  {
                     Err_Manager(Err_NOT_SUPPORTED_TYPE,CREATE);
                     if(!commandArgs[2]) {Err_Manager(Err_NO_SPECIFIED_PATH_TO_NAME,CREATE);}
                  }

            } else {Err_Manager(Err_Usage,CREATE);}
        }

        /*02.$Delete -[Type] -[PATH/TO/SPECIFIED-NAME]*/
        else if(strcmp(getLower(commandArgs[0]),"delete")==0)
        {
            if(commandArgs[1])
            {
                if( (strcmp(getLower(commandArgs[1]),"-d")==0) || (strcmp(getLower(commandArgs[1]),"-f")==0)
                  || (strcmp(getLower(commandArgs[1]),"d")==0) || (strcmp(getLower(commandArgs[1]),"f")==0) )
                {
                    if(commandArgs[2])
                    {
                        if(_Delete(commandArgs[1],commandArgs[2])==-1) {Err_Manager(Err_COULD_NOT,DELETE);}
                    } else {Err_Manager(Err_NO_SPECIFIED_PATH_TO_NAME,DELETE);}

                } else
                  {
                     Err_Manager(Err_NOT_SUPPORTED_TYPE,DELETE);
                     if(!commandArgs[2]) {Err_Manager(Err_NO_SPECIFIED_PATH_TO_NAME,DELETE);}
                  }

            } else {Err_Manager(Err_Usage,DELETE);}
        }

        /*03.$changeName -[PATH/TO/OLD-NAME] -[NEW-NAME]*/
        else if(strcmp(getLower(commandArgs[0]),"changename")==0)
        {
            if(commandArgs[1])
            {
                if(commandArgs[2])
                {
                    if(changeName(commandArgs[1],commandArgs[2])==-1) {Err_Manager(Err_COULD_NOT,CHANGE_NAME);}
                } else {Err_Manager(Err_NO_SPECIFIED_NEW_NAME,CHANGE_NAME);}

            } else {Err_Manager(Err_Usage,CHANGE_NAME);}
        }


        /*04.$Copy -[PATH/TO/SRC_NAME] -[PATH/TO/NEW_COPY]*/
        else if( (strcmp(getLower(commandArgs[0]),"makecopy")==0) || ((strcmp(getLower(commandArgs[0]),"copy")==0)) )
        {
            if(commandArgs[1])
            {
                if(commandArgs[2])
                {
                    if(makeCopy(commandArgs[1],commandArgs[2])==-1) {Err_Manager(Err_COULD_NOT,MAKE_COPY);}
                } else {Err_Manager(Err_NO_SPECIFIED_NEW_NAME,MAKE_COPY);}

            } else {Err_Manager(Err_Usage,MAKE_COPY);}
        }


        /*05.$Encrypt -[PATH/TO/SRC_NAME]*/
        else if(strcmp(getLower(commandArgs[0]),"encrypt")==0)
        {
            if(commandArgs[1])
            {
                if(encrypt(commandArgs[1])==-1) {Err_Manager(Err_COULD_NOT,ENCRYPT);}

            } else {Err_Manager(Err_Usage,ENCRYPT);}
        }


        /*06.$Decrypt -[PATH/TO/SRC_NAME]*/
        else if(strcmp(getLower(commandArgs[0]),"decrypt")==0)
        {
            if(commandArgs[1])
            {
                if(_Decrypt(commandArgs[1])==-1) {Err_Manager(Err_COULD_NOT,DECRYPT);}

            } else {Err_Manager(Err_Usage,DECRYPT);}
        }


        /*
            ||\ TO DO /||

            07.$ListCurrentDir

            08.$changeDir -[PATH/TO/NEW_DIR]
        */
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

        else if(waitpid(proc_Id,status,0)==proc_Id) {/* What's 1000-7 ? */}
        else {printf("Err\n");_exit(0);}
    }

    return 0;
}
