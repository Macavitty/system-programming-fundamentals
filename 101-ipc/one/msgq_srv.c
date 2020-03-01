#include <string.h>
#include <sys/msg.h>
#include "../model/srv.h"
#include "../model/msgq.h"


info_t* server_info;
msg_t* msg;
ping_msg_t* ping_msg;
key_t qkey;
int qid;

int init_q();
void interrupt (int sig);
void release_res();

int main() {
    signal(SIGINT, interrupt);
    signal(SIGHUP, interrupt);

    init_server_info();
    if (init_q() < 0){
        perror ("init queue");
        release_res();
        return EXIT_FAILURE;
    }

    server_info = malloc(sizeof(info_t));
    msg = malloc(sizeof(msg_t));
    ping_msg = malloc(sizeof(ping_msg_t));

    set_color(yellow);
    printf("* Server started *\nUsing System V message queue.\n");

    set_server_info(server_info);
    while (1){
        // get request
        if (msgrcv(qid, ping_msg, sizeof(ping_msg_t), MSG_FOR_SRV_TYPE, 0) != -1) {
            if (strcmp(ping_msg->content, "ping") == 0){
                printf("Got request.\n");

                msg->msg_type = MSG_FOR_CLNT_TYPE;
                memcpy(&(msg->content), server_info, sizeof(info_t));

                // send response
                if (msgsnd(qid, msg, sizeof(msg_t), 0) == -1) {
                    fprintf(stderr, "%s\n", strerror(errno));
                }
            }
        }

        upd_server_info(server_info);
        sleep(1);
    }
}

int init_q(){
    if (init_server(MSGQ_IPC_TYPE) < 0)
        return -1;

    if ((qkey = ftok (get_key_filepath(), get_pjid())) == -1)
        return -1;

    if ((qid = msgget(qkey, IPC_CREAT | PERMISSION)) == -1)
        return -1;
    return 0;
}

void release_res(){
    free(server_info);
    free(ping_msg);
    free(msg);
    msgctl(qid, IPC_RMID, NULL);
}

void interrupt (int sig){
    release_res();
    set_color(yellow);
    printf("\nGot interrupt signal:%d \n", sig);
    printf("Resources released.\n");
    printf("* Server stopped *\n");
    set_color(def);
    exit(EXIT_SUCCESS);
}

