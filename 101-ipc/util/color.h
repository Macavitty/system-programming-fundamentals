#ifndef IPC_LAB_COLOR_H
#define IPC_LAB_COLOR_H
#include <stdio.h>

#define RED "\033[1;31m"
#define GREEN "\033[0;32m"
#define GREEN_BOLD "\033[1;32m"
#define YELLOW "\033[0;33m"
#define BLUE "\x1b[34m"
#define WHITE "\033[1;37m"
#define MAGENTA "\x1b[35m"
#define  DEF "\033[0m"


enum color{
    red, green, green_bold, yellow, blue, def, white, magenta
};

void set_color(enum color c);

#endif //IPC_LAB_COLOR_H
