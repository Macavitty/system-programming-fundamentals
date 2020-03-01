#ifndef IPC_LAB_MSGQ_H
#define IPC_LAB_MSGQ_H

#include "server_info.h"

#define MSG_FOR_SRV_TYPE 41
#define MSG_FOR_CLNT_TYPE 42

typedef struct msg {
    long msg_type;
    info_t content;
} msg_t;

typedef struct ping_msg {
    long msg_type;
    char content[4];
} ping_msg_t;

#endif //IPC_LAB_MSGQ_H
