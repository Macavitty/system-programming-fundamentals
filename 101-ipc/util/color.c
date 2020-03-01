#include "color.h"

void set_color(enum color c){
    switch(c){
        case red: printf(RED); return;
        case green: printf(GREEN); return;
        case green_bold: printf(GREEN_BOLD); return;
        case yellow: printf(YELLOW); return;
        case blue: printf(BLUE); return;
        case white: printf(WHITE); return;
        case magenta: printf(MAGENTA); return;
        case def:
        default: printf(DEF); return;
    }
}