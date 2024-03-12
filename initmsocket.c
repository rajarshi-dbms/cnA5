#include "msocket.h"
#include <pthread.h>
#include <sys/select.h>



sem_t semaphore1;
sem_t semaphore2;

// Thread R Function
void *thread_R(void *arg)
{
  
}

// Thread S Function
void *thread_S(void *arg)
{
   
}






int main()
{
    // Initialize MTP sockets, bind, etc.
    key_t key;
    int shmid;
   SOCK_INFO *shared_info;

    // Generate key using ftok
    if ((key = ftok(SHM_KEY_PATH, SHM_KEY_ID)) == -1) {
        perror("ftok");
        exit(EXIT_FAILURE);
    }

    // Create the shared memory segment
    if ((shmid = shmget(key, sizeof(SOCK_INFO), IPC_CREAT | 0666)) == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    // Attach the shared memory segment to our data structure
    if ((shared_info = (SOCK_INFO *)shmat(shmid, NULL, 0)) == (SOCK_INFO *)-1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }
    // Initialize the shared structure
    shared_info->sock_id = 0;
    strcpy(shared_info->IP, "0.0.0.0");
    shared_info->port = 0;
    shared_info->error_code = 0;

    // Detach the shared memory segment
    if (shmdt(shared_info) == -1) {
        perror("shmdt");
        exit(EXIT_FAILURE);
    }

    printf("Shared memory segment created and initialized successfully.\n");





    return 0;



}