#include "ERROR_MANAGER.h"

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
            else if(Command_ID==CHANGE_CD)
             {printf("\nCOMMAND_ERROR :\n\t -Try $ChangeCD -[PATH/TO/DIR]\n\n");}
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
            else if(Command_ID==DECRYPT)
             {printf("\nCOMMAND_ERROR : _NOT_EXISTS \n\n\t-Try $Decrypt -[PATH/TO/SRC_NAME]\n\n");}

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
            else if(Command_ID==LIST_CD)
             {printf("\nCOMMAND_ERROR : _COULD_NOT_LIST_CD \n\n\t-Try $ListCD\n\n");}
            else if(Command_ID==CHANGE_CD)
             {printf("\nCOMMAND_ERROR : _COULD_NOT_CHANGE_CD \n\n\t-Try $ChangeCD -[PATH/TO/DIR]\n\n");}
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

        /*$Decrypt -[PATH/TO/SRC_NAME]*/
        case Err_ON_DELETE :
            {printf("\nCOMMAND_ERROR : _COULD_NOT_DELETE BACKUP_TEMP \n");}

        /*Defual*/
        default :
            break;

    }
}
