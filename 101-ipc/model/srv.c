#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <math.h>
#include <string.h>
#include <fcntl.h>
#include "srv.h"

time_t start;
char* key_file;

int get_key_num(){
    return getpid();
}

int get_pjid(){
    int id = getpid();
    while((id & 0xFF) != 0){
        id >>= 1;
    }
    return id;
}

const  char* get_key_filepath(){
    return key_file;
}

void set_key_filepath(const char * ipc_type){
    int key = get_key_num();
    char* slash = "/";
    char* suffix = malloc(floor(log10(key) + 2)*sizeof(char));
    sprintf(suffix, "%d", key);
    key_file = malloc(strlen(KEYS_DIR) + strlen(slash) + strlen(suffix));
    strcpy(key_file, KEYS_DIR);
    strcat(key_file, slash);
    strcat(key_file, ipc_type);
    strcat(key_file, suffix);
}

void init_server_info(){
    start = time(NULL);
}

void set_server_info(info_t * info){
    info->pid = getpid();
    info->uid = getuid();
    info->gid = getgid();
    info->time_run = time(NULL) - start;
    getloadavg(info->loadavg, 3);
}

void upd_server_info(info_t * info){
    info->time_run = time(NULL) - start;
    getloadavg(info->loadavg, 3);
}

int init_server(const char * ipc_type){
    struct stat st = {0};
    if (stat(KEYS_DIR, &st) == -1) {
        mkdir(KEYS_DIR, 0777);
    }
    set_key_filepath(ipc_type);
    return open(key_file, O_RDWR | O_CREAT, 0640);;
}

int deinit_server(){
    return remove(key_file);
}

void print_pid(info_t * info) { printf("PID: %i\n", info->pid); }
void print_uid(info_t * info) { printf("UID: %i\n", info->uid); }
void print_gid(info_t * info) { printf("GID: %i\n", info->gid); }
void print_time(info_t * info) { printf("Server runs for %li seconds\n", info->time_run); }
void print_load(info_t * info) { printf("System load averages over the last:\n \t1 minute - %.3lf,\n \t5 minutes - %.3lf,\n \t15 minutes - %.3lf\n",
                                        info->loadavg[0], info->loadavg[1], info->loadavg[2]); }

