#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <stdlib.h>
#include "../model/letters.h"

#define PERMS 0600
#define INIT_SEMVAL 1

int sid;

struct sembuf dec = {1, -1, 0};
struct sembuf inc = {1, 1, 0};
struct sembuf zero = {1, 0, 0};

void wait();
void post();
void *change_case();
void *reverse();

int flag = 1;

char letters[LETTERS_N] = ALPHABET;

int main() {
    pthread_t reverse_thr;
    pthread_t case_thr;

    if ((sid = semget(IPC_PRIVATE, 2, IPC_CREAT | PERMS)) == -1) {
        perror("semget");
        return EXIT_FAILURE;
    }

    // set initial  value for a semaphore
    semctl(sid, 1, SETVAL, INIT_SEMVAL);

    // create threads
    if (pthread_create(&reverse_thr, NULL, reverse, NULL) != 0 ||
        pthread_create(&case_thr, NULL, change_case, NULL) != 0) {
        perror("pthread_create");
        return EXIT_FAILURE;
    }

    printf("Press any key to exit\n");

    getchar();
    flag = -1;
    printf("Blocked semaphore\n");
    semop(sid, &inc, 1);

    // start
    pthread_join(reverse_thr, NULL);
    pthread_join(case_thr, NULL);

    // remove the semaphore set
    semctl(sid, 0, IPC_RMID);
    return EXIT_SUCCESS;
}

void *change_case() {
    while (1) {
        wait();
        if (flag < 0){
            //pthread_exit(0);
            return 0;
        }
        change_case_algo(letters);
        sleep(1);
        printf("Change case:\t%s\n", letters);
        post();
        sleep(1);
    }
}

void *reverse() {
    while (1) {
        wait();
        if (flag < 0){
            pthread_exit(0);
        }
        reverse_order_algo(letters);
        sleep(1);
        printf("Reverse:\t%s\n", letters);
        post();
        sleep(1);
    }
}

void wait() {
    if (semop(sid, &dec, 1) == -1)
        perror("semop: wait");
}

void post() {
    while (semop(sid, &zero, 1));
    semop(sid, &inc, 1);
}