#include <sys/msg.h>
#include <string.h>
#include "../model/msgq.h"
#include "../model/clnt.h"

int main() {
    msg_t *msg;
    ping_msg_t *ping_msg;
    key_t key;
    int qid;

    if (find_working_server(MSGQ_IPC_TYPE) < 0){
        printf("No running server found\n");
        return EXIT_SUCCESS;
    }

    if ((key = ftok(get_key_filepath(), get_pjid())) == -1){
        perror("ftok");
        return EXIT_FAILURE;
    }

    if ((qid = msgget(key, 0)) < 0) {
        perror("msgget");
        return EXIT_FAILURE;
    }

    msg = malloc(sizeof(msg_t));
    ping_msg = malloc(sizeof(ping_msg_t));

    ping_msg->msg_type = MSG_FOR_SRV_TYPE;
    strcpy(ping_msg->content, "ping");

    if (msgsnd(qid, ping_msg, sizeof(ping_msg_t), 0) == -1) {
        perror("msgsnd");
        return EXIT_FAILURE;
    }

    if (msgrcv(qid, msg, sizeof(msg_t), MSG_FOR_CLNT_TYPE, 0) == -1) {
        perror("msgrcv");
        return EXIT_FAILURE;
    }

    print_server_info(&(msg->content));
    return EXIT_SUCCESS;
}