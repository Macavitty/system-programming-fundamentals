#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <math.h>
#include "server_info.h"
#include "clnt.h"

int srv_pid = -1;
char* key_file;

int get_key_num(){
    return srv_pid;
}

int get_pjid(){
    int id = srv_pid;
    while((id & 0xFF) != 0){
        id >>= 1;
    }
    return id;
}

const  char* get_key_filepath(){
    return key_file;
}

void print_server_info(info_t * info){
    set_color(green);
    printf("\nServer info:\n");
    printf("PID: %d\n", info->pid);
    printf("UID: %d\n", info->uid);
    printf("GID: %d\n", info->gid);
    printf("time running: %ld\n", info->time_run);
    printf("system load averages over the last 1 minute:\t%f\n", info->loadavg[0]);
    printf("system load averages over the last 5 minutes:\t%f\n", info->loadavg[1]);
    printf("system load averages over the last 15 minutes:\t%f\n", info->loadavg[2]);
    set_color(def);
}

void set_srv_pid(int pid){
    srv_pid = pid;
}

void set_key_filepath(const char * suffix){
    char* slash = "/";
    key_file = malloc(strlen(KEYS_DIR) + strlen(slash) + strlen(suffix));
    strcpy(key_file, KEYS_DIR);
    strcat(key_file, slash);
    strcat(key_file, suffix);
}

int find_working_server(const char* ipc_type){
    DIR *d;
    struct dirent *dir;
    d = opendir(KEYS_DIR);
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            if(strncmp(dir->d_name, ipc_type, strlen(ipc_type)) == 0) {
                char *arg_end;
                int pid = (int) strtol(dir->d_name + strlen(ipc_type), &arg_end, 10);
                if (*arg_end == '\0'){
                    set_key_filepath(dir->d_name);
                    set_srv_pid(pid);
                    break;
                }
            }
        }
        closedir(d);
    }
    return srv_pid;
}