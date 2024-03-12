#include "msocket.h"


sem_t semaphore1;
sem_t semaphore2;


int m_socket(int domain, int type, int flag){

    if (type != SOCK_MTP)
    {
        perror("socket type was wrong");
        exit(1);
    }

     

    

    
}
int main() {
    key_t key;
    int shmid;
    SOCK_INFO *shared_info;

    // Generate the same key using ftok
    if ((key = ftok(SHM_KEY_PATH, SHM_KEY_ID)) == -1) {
        perror("ftok");
        exit(EXIT_FAILURE);
    }

    // Get the identifier for the shared memory segment
    if ((shmid = shmget(key, sizeof(SOCK_INFO), 0666)) == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    // Attach the shared memory segment to our data structure
    if ((shared_info = (SOCK_INFO *)shmat(shmid, NULL, 0)) == (SOCK_INFO *)-1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    // Access and modify the shared data
    printf("sock_id: %d\n", shared_info->sock_id);
    printf("IP: %s\n", shared_info->IP);
    printf("port: %d\n", shared_info->port);
    printf("error_code: %d\n", shared_info->error_code);

    // Detach the shared memory segment
    if (shmdt(shared_info) == -1) {
        perror("shmdt");
        exit(EXIT_FAILURE);
    }

    return 0;
}
