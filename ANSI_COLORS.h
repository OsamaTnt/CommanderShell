/* written by Osama Tnt 21/6/018 */

/*
    ANSI_COLOR CODES Will be used to proc $changeTextColor
*/
#include "stdio.h"
#include "string.h"

#define ANSI_BLACK "\033[22;30m"
#define ANSI_RED "\033[22;31m"
#define ANSI_GREEN "\033[22;32m"
#define ANSI_BROWN "\033[22;33m"
#define ANSI_BLUE "\033[22;34m"
#define ANSI_MAGENTA "\033[22;35m"
#define ANSI_CYAN "\033[22;36m"
#define ANSI_GRAY "\033[22;37m"
#define ANSI_YELLOW "\033[01;33m"
#define ANSI_WHITE "\033[01;37m"
#define ANSI_DARK_GRAY "\033[01;30m"
#define ANSI_LIGHT_RED "\033[01;31m"
#define ANSI_LIGHT_GREEN "\033[01;32m"
#define ANSI_LIGHT_BLUE "\033[01;34m"
#define ANSI_LIGHT_MAGENTA "\033[01;35m"
#define ANSI_LIGHT_CYAN "\033[01;36m"

char* getLower(char *S);
int changeTextColor(char *COLOR_NAME);
