#define _POSIX_C_SOURCE 2

#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>

#define EXIT_SUCCESS 0
#define EXIT_ILLEGAL_OPT 1
#define EXIT_WITH_SIGINT 130


const char *ENDL = "\n";
bool IGNORE_INTERRUPT = 0;
bool APPEND_MODE = 0;
//int ERRORS_CNT = 0;

const char *ILLEGAL_OPT = "tee: illegal option -- ";
const char *USAGE = "usage: tee [-ai] [file...]\n";

extern int errno;


void interrupt(int sig) {
    if (!IGNORE_INTERRUPT) {
        write(1, ENDL, strlen(ENDL));
        exit(EXIT_WITH_SIGINT);
    }
}


void tee(const char **files, int size) {
    int *fds = malloc(size + 1);
    fds[0] = 1;
    // open files
    for (int i = 1; i <= size; ++i) {
        if (!APPEND_MODE) {
            fds[i] = open(files[i - 1], O_WRONLY | O_CREAT | O_TRUNC, 0666);
        }
        else {
            fds[i] = open(files[i - 1], O_WRONLY | O_APPEND | O_CREAT);
        }
    }

    // read
    char c;
    while (read(0, &c, 1) > 0) {
        for (int i = 0; i <= size; i++) {
            write(fds[i], &c, 1);
        }
    }

    // close files
    for (int i = 1; i <= size; ++i)
        close(fds[i]);

    free(fds);
    free(files);
}



int main(int argc, char *argv[]) {
    char *opt_keys = "ai";
    errno = 0;

    signal(SIGINT, interrupt);

    int opt;
    opterr = 0;
    while ((opt = getopt(argc, argv, opt_keys)) != -1) {
        switch (opt) {
            case 'a':
                APPEND_MODE = 1;
                break;
            case 'i':
                IGNORE_INTERRUPT = 1;
                break;
            default: {
                if (opt != -1) {
                    char cc = optopt;
                    write(2, ILLEGAL_OPT, strlen(ILLEGAL_OPT));
                    write(2, &cc, sizeof(cc));
                    write(2, ENDL, strlen(ENDL));
                    write(2, USAGE, strlen(USAGE));
                    exit(EXIT_ILLEGAL_OPT);
                }
            }
        }
    }
    char **const files = malloc(argc - optind);
    for (int i = optind; i < argc; ++i) {
        files[i - optind] = malloc(strlen(argv[i]));
        strcpy(files[i - optind], argv[i]);
    }
    tee((const char **) files, argc - optind);
    return EXIT_SUCCESS;
}


