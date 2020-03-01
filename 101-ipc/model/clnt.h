#ifndef IPC_LAB_CLNT_H
#define IPC_LAB_CLNT_H

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include "server_info.h"
#include "../util/color.h"

void print_server_info(info_t * info);
int get_pjid();
const char* get_key_filepath();
int find_working_server(const char* ipc_type);
#endif //IPC_LAB_CLNT_H
