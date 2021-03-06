#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <dirent.h>
#include <time.h>
#include "ERROR_MANAGER.h"
#include "ANSI_COLORS.h"

const int MAX_ARGS_SIZE =15;
const int MAX_PATH_LENGTH=512;
size_t i,j;
bool bshowCD;

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

/*
    changeTextColor() ".How it Works." :
    if ( COLOR_NAME IS CORRECT && COLOR_IS_SUPPORTED) {UPDATE_COLOR && return(0)}
    else {Err && return(-1)}
*/
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
    int Pos_To_Last=0;
    bool bIsPath=false;

    for(i=0;i<strlen(PATH_TO_NAME);i++)
    {
        if(PATH_TO_NAME[i]=='/') {bIsPath=true; Pos_To_Last=i;}
    }

    if(!bIsPath) {LAST_NAME = PATH_TO_NAME;}
    else
    {
        LAST_NAME = malloc(strlen(PATH_TO_NAME)-Pos_To_Last);

        for(i=0,j=Pos_To_Last+1;j<=strlen(PATH_TO_NAME);i++,j++)
        {LAST_NAME[i] = PATH_TO_NAME[j];}
    }

    return LAST_NAME;
}

void removeLastFromPath(char *PATH_TO_NAME)
{
    int Pos_To_Last=0;
    bool bIsPath=false;

    for(i=0;i<strlen(PATH_TO_NAME);i++)
    {
        if(PATH_TO_NAME[i]=='/') {bIsPath=true; Pos_To_Last=i;}
    }

    //nothing to be removed
    if(!bIsPath) {return;}
    else {PATH_TO_NAME[Pos_To_Last]='\0';}
}

char *getCurrentDir()
{
    char *currentDir=(char*)malloc(MAX_PATH_LENGTH);
    getcwd(currentDir,MAX_PATH_LENGTH);
    return currentDir;
}

char *getBackupID() { return "-d@"; }

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
        char *BACKUP_ID=getBackupID();

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
    FILE *srcFile=fopen(PATH_TO_NAME,"r+");
    if(srcFile)
    {
        char *BACKUP_Name=(char *)malloc(strlen(getLastFromPath(PATH_TO_NAME))+strlen(getBackupID()));
        strcat(BACKUP_Name,getBackupID());
        strcat(BACKUP_Name,getLastFromPath(PATH_TO_NAME));

        char *BACKUP_PATH=(char *)malloc(MAX_PATH_LENGTH);
        removeLastFromPath(PATH_TO_NAME);

        if(bIsDirExists(PATH_TO_NAME)) {strcat(BACKUP_PATH,PATH_TO_NAME);}
        else {strcat(BACKUP_PATH,getCurrentDir());}

        strcat(BACKUP_PATH,"/BackupFolder");

        if(!bIsDirExists(BACKUP_PATH)) {return -1;}

        strcat(BACKUP_PATH,"/"); strcat(BACKUP_PATH,BACKUP_Name);

        FILE *d_srcFile=fopen(BACKUP_PATH,"r");
        if(!d_srcFile || !bIsFileExists(BACKUP_PATH)) {return -1;}

        char c;
        while( (c=fgetc(d_srcFile))!=EOF )
        {fputc(c,srcFile);} fclose(d_srcFile); fclose(srcFile);

        if(!unlink(BACKUP_PATH)) { Err_Manager(Err_ON_DELETE,DECRYPT); }
        return 0;
    }

    else {Err_Manager(Err_NOT_EXISTS,DECRYPT);}
    return 0;
}

int listCD()
{
    char *CD_NAME=getLastFromPath(getCurrentDir());
    printf("\nCD : %s\n",CD_NAME);

    DIR *CD = opendir(getCurrentDir());
    struct dirent *CD_content=NULL;
    struct stat stat_buf;

    if(!CD) {return -1;}

    printf("\n---------------------------------------------------------------------------");
    printf("\n\tContent \t\t Size \n");
    printf("---------------------------------------------------------------------------\n");

    int max_space_lenght=0;
    while( (CD_content=readdir(CD))!=NULL )
    {
        if(max_space_lenght<strlen(CD_content->d_name))
        {max_space_lenght=strlen(CD_content->d_name);}
    }closedir(CD);

    if( !(CD=opendir(getCurrentDir())) ) {return -1;}

    while( (CD_content=readdir(CD))!=NULL )
    {
        printf("%s",CD_content->d_name);

        for(i=0;i<=max_space_lenght-strlen(CD_content->d_name);i++)
        {printf(" ");}

        stat(CD_content->d_name,&stat_buf);
        printf("\t-\t %iB\n",(int)stat_buf.st_size);

    } printf("---------------------------------------------------------------------------\n\n");
     closedir(CD);

    return 0;
}
/*
    "what happens in case of $changeCD (THE Problema "important" ".--.") "
     -when a new process is spawned to run $changeCD command
     -the chdir() will change its current directory
     -Once the child exits(died), we're back to the parent shell current directory.
     -The shell is still where it was
*/
int changeCD(char *PATH_TO_DIR)
{
    if(!bIsDirExists(PATH_TO_DIR)) {printf("wrong\n");return -1;}

    chdir(PATH_TO_DIR);
    bshowCD=true;

    char *CD_NAME=getLastFromPath(getCurrentDir());
    printf("\nCD : %s\n\n",CD_NAME);

    return 0;
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


        /*07.$ListCD*/
        //TO DO seperate the commandArgs[0] && commandArgs[1] ==> /*07.$List -[CD || PATH/TO/DIR]*/
        else if(strcmp(getLower(commandArgs[0]),"listcurrentdirectory")==0 || strcmp(getLower(commandArgs[0]),"listcd")==0)
        { if(listCD()==-1) {Err_Manager(Err_COULD_NOT,LIST_CD);}}


        /*08.$ChangeCD -[PATH/TO/NEW_DIR]*/
        else if(strcmp(getLower(commandArgs[0]),"changecurrentdirectory")==0 || strcmp(getLower(commandArgs[0]),"changecd")==0 || strcmp(getLower(commandArgs[0]),"changedir")==0)
        {
            if(commandArgs[1])
            {
                if(changeCD(commandArgs[1])==-1) {Err_Manager(Err_COULD_NOT,CHANGE_CD);}
            } else {Err_Manager(Err_Usage,CHANGE_CD);}
        }
    }

}

int main()
{
    bshowCD=false;
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
