#ifndef IPC_LAB_SRV_H
#define IPC_LAB_SRV_H

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <signal.h>
#include <errno.h>
#include "../util/color.h"
#include "server_info.h"

void init_server_info();
void set_server_info(info_t * info);
void upd_server_info(info_t * info);
void print_pid(info_t * info);
void print_uid(info_t * info);
void print_gid(info_t * info);
void print_time(info_t * info);
void print_load(info_t * info);
int init_server(const char * ipc_type);
int deinit_server();
int get_pjid();
const char* get_key_filepath();

#endif //IPC_LAB_SRV_H
