#ifndef IPC_LAB_SERVER_INFO_H
#define IPC_LAB_SERVER_INFO_H

#include <time.h>
#include <sys/types.h>
#include <unistd.h>

#define KEYS_DIR "/tmp/osp_ipc"
#define SHM_IPC_TYPE "shm"
#define MSGQ_IPC_TYPE "msgq"
#define PERMISSION 0644

typedef struct server_info {
    pid_t pid;
    uid_t uid;
    gid_t gid;
    time_t time_run;
    double loadavg[3];
} info_t;

#endif //IPC_LAB_SERVER_INFO_H


/*
 * Task
 * */