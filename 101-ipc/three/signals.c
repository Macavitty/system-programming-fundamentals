#include <signal.h>

#include "../model/server_info.h"
#include "../model/srv.h"

info_t* info;
struct sigaction *act;

void handler(int signum){
    if (signum == SIGHUP) print_pid(info);
    if (signum == SIGINT) print_uid(info);
    if (signum == SIGTERM) print_gid(info);
    if (signum == SIGUSR1) print_time(info);
    if (signum == SIGUSR2) print_load(info);
}

int sigact(){
    act = malloc(sizeof(struct sigaction));
    act->sa_handler = handler;
    if (sigaction(SIGHUP, act, NULL) != 0) return -1;
    if (sigaction(SIGINT, act, NULL) != 0) return -1;
    if (sigaction(SIGTERM, act, NULL) != 0) return -1;
    if (sigaction(SIGUSR1, act, NULL) != 0) return -1;
    if (sigaction(SIGUSR2, act, NULL) != 0) return -1;
    return 0;
}

int main(){
    init_server_info();

    info = malloc(sizeof(info_t));
    set_server_info(info);
    printf("* Started server *\n");
    print_pid(info);

    if (sigact() == -1){
        perror("sigaction");
        exit(EXIT_SUCCESS);
    }
    while(1){
        upd_server_info(info);
        sleep(1);
    }
}

