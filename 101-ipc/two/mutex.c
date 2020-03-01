#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include "../util/color.h"
#include "../model/letters.h"

#define MILLI 1000

long time_main = 800*MILLI;
long time_reverse = 800*MILLI;
long time_case = 800*MILLI;

char letters[LETTERS_N] = "abcdefghijklmnopqrstuvwxyz";

pthread_mutex_t lock;

void* change_case();
void* reverse();
long parse_optarg_long(const char *arg);

int main(int argc, char *argv[]) {
    char *opt_keys = "m:r:c:";

    int opt;
    opterr = 0;
    while ((opt = getopt(argc, argv, opt_keys)) != -1) {
        switch (opt) {
            case 'm':
                time_main = parse_optarg_long(optarg);
                break;
            case 'r':
                time_reverse = parse_optarg_long(optarg);
                break;
            case 'c':
                time_case = parse_optarg_long(optarg);
                break;
            default:
                perror("opt");
                return EXIT_FAILURE;
        }
    }

    pthread_mutex_init(&lock, NULL);

    pthread_t reverse_thr;
    pthread_t case_thr;

    // create threads
    if (pthread_create(&reverse_thr, NULL, reverse, NULL) != 0 ||
        pthread_create(&case_thr, NULL, change_case, NULL) != 0) {
        perror("pthread_create");
        return EXIT_FAILURE;
    }
    set_color(magenta);
    while(1){
        pthread_mutex_lock(&lock);
        printf("NOW:\t%s\n\n", letters);
        pthread_mutex_unlock(&lock);
        usleep(time_main);
    }
    set_color(def);
    return EXIT_SUCCESS;
}

void* change_case() {
    while (1) {
        pthread_mutex_lock(&lock);
        for (int i = 0; i < LETTERS_N; ++i) {
            char c = letters[i];
            if (c >= 'A' && c <= 'Z') {
                letters[i] = (char) (letters[i] + 32);
            }
            else if (c >= 'a' && c <= 'z') {
                letters[i] = (char) (letters[i] - 32);
            }
        }
        pthread_mutex_unlock(&lock);
        usleep(time_case);
    }
}

void* reverse() {
    while (1) {
        pthread_mutex_lock(&lock);
        for (int i = 0; i < LETTERS_N/2; i++) {
            char tmp  = letters[i];
            letters[i] = letters[LETTERS_N - 1 - i];
            letters[LETTERS_N - 1 - i] = tmp;
        }
        pthread_mutex_unlock(&lock);
        usleep(time_reverse);
    }
}

long parse_optarg_long(const char *arg) {
    char *arg_end;
    long num;
    if (arg[0] == '-') {
        exit(EXIT_FAILURE);
    }
    num = strtol(arg, &arg_end, 10);
    if (*arg_end != '\0') {
        fprintf(stderr, "Arguments must be positive numbers");
        exit(EXIT_FAILURE);
    }
    return num;
}