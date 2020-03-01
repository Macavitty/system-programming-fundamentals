#include "../model/srv.h"
#include <sys/shm.h>

info_t* server_info;
key_t shm_key;
int shm_id;

int init_shm();
void interrupt (int sig);
void release_res();

int main() {
    signal(SIGINT, interrupt);
    signal(SIGHUP, interrupt);

    init_server_info();
    if (init_shm() < 0){
        perror("init_shm");
        release_res();
        return EXIT_FAILURE;
    }

    set_color(blue);
    printf("* Server started *\nUsing System V shared memory\n");
    set_color(def);

    set_server_info(server_info);
    while (1){
        upd_server_info(server_info);
        sleep(1);
    }
}

int init_shm(){
    if (init_server(SHM_IPC_TYPE) < 0)
        return -1;

    if ((shm_key = ftok (get_key_filepath(), get_pjid())) == -1)
        return -1;

    if ((shm_id = shmget(shm_key, sizeof(info_t), IPC_CREAT | PERMISSION)) == -1)
        return -1;

    // put info in mem
    if ((server_info = (info_t*) shmat(shm_id, NULL, 0)) == NULL)
        return -1;
    return 0;
}

void release_res(){
    shmdt(server_info);
    shmctl(shm_id, IPC_RMID, NULL);
    deinit_server();
}

void interrupt (int sig){
    release_res();

    set_color(blue);
    printf("\nGot interrupt signal:%d \n", sig);
    printf("Resources released.\n");
    printf("* Server stopped *\n");
    set_color(def);
    exit(EXIT_SUCCESS);
}
