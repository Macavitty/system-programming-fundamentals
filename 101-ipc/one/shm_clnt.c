#include <sys/shm.h>
#include "../model/clnt.h"

info_t* server_info;
key_t shm_key;
int shm_id;

int main(){
    if (find_working_server(SHM_IPC_TYPE) < 0){
        printf("No running server found\n");
        return EXIT_SUCCESS;
    }
    if ((shm_key = ftok (get_key_filepath(), get_pjid())) == -1){
        perror ("ftok");
        return EXIT_FAILURE;
    }

    if ((shm_id = shmget(shm_key, sizeof(info_t), IPC_CREAT | PERMISSION)) == -1){
        perror ("shmget");
        return EXIT_FAILURE;
    }

    if ((server_info = (info_t*) shmat(shm_id, NULL, 0)) == NULL){
        perror ("shmat");
        return EXIT_FAILURE;
    }

    print_server_info(server_info);
}