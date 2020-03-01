#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]){

    // put args into command
    if (argc < 2) {
        return 1;
    }
    char* command = (char*) malloc(sizeof(argv[1]) + sizeof(argv[2]));

    strcpy(command, argv[1]);
    strcat(command, " ");
    for (int i = 2; i < argc; i++){
        strcat(command, argv[i]);
        strcat(command, " ");
    }

    // get input from stdin
    char buffer[256];
    while (fgets(buffer, 256, stdin)) {
        strcat(command, buffer);
    }

    // remove endl
    char c;
    size_t i = 0;
    while ((c = command[i++])){
        if ( c == '\n')
            command[i-1] = ' ';
    }

    // run command
    system(command);

    // free
    free(command);
    return 0;
}
