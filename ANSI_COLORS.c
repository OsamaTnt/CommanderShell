#include "ANSI_COLORS.h"

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
