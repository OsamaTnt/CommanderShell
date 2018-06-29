#include "stdio.h"

typedef int bool;
enum {false,true};  //false=0;true=1

enum {
        Err_Usage, Err_COLOR_NAME, Err_NOT_EXISTS, Err_ALREADY_EXISTS,
        Err_NO_SPECIFIED_NEW_NAME, Err_NO_SPECIFIED_PATH_TO_NAME,
        Err_COULD_NOT, Err_NOT_SUPPORTED_TYPE, Err_ON_DELETE
     };


enum {CHANGE_TEXT_COLOR, CREATE,DELETE, CHANGE_NAME,
      MAKE_COPY, ENCRYPT, DECRYPT, LIST_CD, CHANGE_CD};


void Err_Manager(int Err_ID,int Command_ID);
