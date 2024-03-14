#include "msocket.h"
#include <pthread.h>
#include <sys/select.h>
#include "msocket.h"
#include <pthread.h>
#include <sys/select.h>

#define SEM_OP1_INDEX 0
#define SEM_OP2_INDEX 1

int semaphore1,semaphore2;
int semaphore3,semaphore4;
int shmid, shmid1;
SOCK_INFO *shared_info;
MTPSocket *sockM;
pthread_t tid_R, tid_S;
// signal handler CTRL+C

SOCK_INFO *shared_func()
{
    key_t key;
    int shmid;
    SOCK_INFO *shared_info;

    // Generate the same key using ftok
    if ((key = ftok(SHM_KEY_PATH, SHM_KEY_ID)) == -1)
    {
        perror("ftok");
        exit(EXIT_FAILURE);
    }

    // Get the identifier for the shared memory segment
    if ((shmid = shmget(key, sizeof(SOCK_INFO), 0666)) == -1)
    {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    // Attach the shared memory segment to our data structure
    if ((shared_info = (SOCK_INFO *)shmat(shmid, NULL, 0)) == (SOCK_INFO *)-1)
    {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    return shared_info;
}
void end()
{
    printf("\nDeleting Semaphores..........\n");
    semctl(semaphore1, IPC_RMID, 0);
    semctl(semaphore2, IPC_RMID, 0);
    semctl(semaphore3, IPC_RMID, 0);
    semctl(semaphore4, IPC_RMID, 0);
    printf("Deleting shared memory SOCK_INFO and shared_info.......\n");
     // Detach the shared memory segment
    if (shmdt(shared_info) == -1)
    {
        perror("shmdt");
        exit(EXIT_FAILURE);
    }
    // Detach the shared memory segment
    if (shmdt(sockM) == -1)
    {
        perror("shmdt");
        exit(EXIT_FAILURE);
    }
    if (shmctl(shmid, IPC_RMID, NULL) == -1)
    {
        perror("shmctl");
        exit(1);
    }
    if (shmctl(shmid1, IPC_RMID, NULL) == -1)
    {
        perror("shmctl");
        exit(1);
    }
    exit(0);




}
// Thread R Function
void *thread_R(void *arg)
{
    // fd_set ns, rs;
    // FD_ZERO(&ns);

    // for (int i = 0; i < MAX_MTP_SOCKETS; i++)
    // {
    //     if (!sockM[i].free)
    //     {
    //         FD_SET()
    //     }
        
    // }
    


    pthread_exit(NULL);
}

// Thread S Function
void *thread_S(void *arg)
{
    printf("Waiting for signal from sendto\n");
    semaphore_wait(semaphore3);
    printf("Have received \n");    
    semaphore_signal(semaphore4);
    printf("have send signal\n");


    
    pthread_exit(NULL);
}
// Function to initialize semaphore
void semaphore_init(int semaphore_id, int initial_value)
{

    if (semctl(semaphore_id, 0, SETVAL, 0) == -1)
    {
        perror("Semaphore initialization failed");
        exit(EXIT_FAILURE);
    }
}

// Function to perform semaphore wait (P) operation
void semaphore_wait(int semaphore_id)
{
    struct sembuf semaphore_operation;
    semaphore_operation.sem_num = 0; // Index of semaphore in semaphore set
    semaphore_operation.sem_op = -1; // Decrement semaphore value by 1
    semaphore_operation.sem_flg = 0; // Undo the operation if the process terminates
    if (semop(semaphore_id, &semaphore_operation, 1) == -1)
    {
        perror("Semaphore wait (P) operation failed");
        exit(EXIT_FAILURE);
    }
}

// Function to perform semaphore signal (V) operation
void semaphore_signal(int semaphore_id)
{
    struct sembuf semaphore_operation;
    semaphore_operation.sem_num = 0; // Index of semaphore in semaphore set
    semaphore_operation.sem_op = 1;  // Increment semaphore value by 1
    semaphore_operation.sem_flg = 0; // Undo the operation if the process terminates
    if (semop(semaphore_id, &semaphore_operation, 1) == -1)
    {
        perror("Semaphore signal (V) operation failed");
        exit(EXIT_FAILURE);
    }
}
int main()
{

    //////////////////////////////////////semaphore creation/////////////////////////
    key_t sem_key1, sem_key2;
     key_t sem_key3, sem_key4;
    if ((sem_key1 = ftok(SHM_KEY_PATH, SEM_KEY1_ID)) == -1)
    {
        perror("ftok for semaphore1");
        exit(EXIT_FAILURE);
    }

    if ((sem_key2 = ftok(SHM_KEY_PATH, SEM_KEY2_ID)) == -1)
    {
        perror("ftok for semaphore2");
        exit(EXIT_FAILURE);
    }
    if ((sem_key3 = ftok(SHM_KEY_PATH, SEM_KEY3_ID)) == -1)
    {
        perror("ftok for semaphore3");
        exit(EXIT_FAILURE);
    }

    if ((sem_key4 = ftok(SHM_KEY_PATH, SEM_KEY4_ID)) == -1)
    {
        perror("ftok for semaphore4");
        exit(EXIT_FAILURE);
    }

    // Create semaphore 1

    if ((semaphore1 = semget(sem_key1, 1, IPC_CREAT | 0666)) == -1)
    {
        perror("semget for semaphore1");
        exit(EXIT_FAILURE);
    }

    // Create semaphore 2

    if ((semaphore2 = semget(sem_key2, 1, IPC_CREAT | 0666)) == -1)
    {
        perror("semget for semaphore2");
        exit(EXIT_FAILURE);
    }
        if ((semaphore3 = semget(sem_key3, 1, IPC_CREAT | 0666)) == -1)
    {
        perror("semget for semaphore3");
        exit(EXIT_FAILURE);
    }

    // Create semaphore 2

    if ((semaphore4 = semget(sem_key4, 1, IPC_CREAT | 0666)) == -1)
    {
        perror("semget for semaphore4");
        exit(EXIT_FAILURE);
    }

    semaphore_init(semaphore1, 0); // Initializing semaphore value to 0
    semaphore_init(semaphore2, 0); // Initializing semaphore value to 0
    semaphore_init(semaphore3, 0); // Initializing semaphore value to 0
    semaphore_init(semaphore4, 0); // Initializing semaphore value to 0
    printf("Semaphores created and initialized successfully.\n");
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Initialize MTP sockets, bind, etc.
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////
    key_t key, key1;

    // Generate key using ftok
    if ((key = ftok(SHM_KEY_PATH, SHM_KEY_ID)) == -1)
    {
        perror("ftok");
        exit(EXIT_FAILURE);
    }

    // Create the shared memory segment
    if ((shmid = shmget(key, sizeof(SOCK_INFO), IPC_CREAT | 0666)) == -1)
    {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    // Attach the shared memory segment to our data structure
    if ((shared_info = (SOCK_INFO *)shmat(shmid, NULL, 0)) == (SOCK_INFO *)-1)
    {
        perror("shmat");
        exit(EXIT_FAILURE);
    }
    // Initialize the shared structure
    shared_info->sock_id = 0;
    strcpy(shared_info->IP, "");
    shared_info->port = 0;
    shared_info->errorno = 0;

    printf("Shared memory segment created and initialized successfully.\n");

    // Generate key using ftok
    if ((key1 = ftok(MTP_KEY_PATH, MTP_KEY_ID)) == -1)
    {
        perror("ftok");
        exit(EXIT_FAILURE);
    }

    // Create the shared memory segment
    if ((shmid1 = shmget(key1, sizeof(MTPSocket) * MAX_MTP_SOCKETS, IPC_CREAT | 0666)) == -1)
    {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    // Attach the shared memory segment to our MTPSocket array
    if ((sockM = (MTPSocket *)shmat(shmid1, NULL, 0)) == (MTPSocket *)-1)
    {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    // Initialize the shared MTPSocket array
    for (int i = 0; i < MAX_MTP_SOCKETS; ++i)
    {
        sockM[i].free = true;
        sockM[i].pid = 0;
        sockM[i].UDPsocID = -1;
        memset(sockM[i].des_IP, 0, sizeof(sockM[i].des_IP));
        sockM[i].des_port = 0;
        memset(sockM[i].source_IP, 0, sizeof(sockM[i].source_IP));
        sockM[i].source_port = 0;
        memset(sockM[i].sbuf, 0, sizeof(sockM[i].sbuf));
        memset(sockM[i].rbuf, 0, sizeof(sockM[i].rbuf));
        sockM[i].nospace = false;
        sockM[i].ack_num = -1;
        sockM[i].sendNospace = false;
        sockM[i].l_send = -1;
        sockM[i].f_send = -1;
        // Initialize other fields as needed...
    }

   
    pthread_create(&tid_R, NULL, thread_R, NULL);
    pthread_create(&tid_S, NULL, thread_S, NULL);
    
   


    signal(SIGINT,end);

    while (1)
    {
      

        printf("waiting for signal from msocket.c\n");
        // Wait for a signal on semaphore 1
        semaphore_wait(semaphore1); // Perform wait operation

        if (shared_info->sock_id == 0 && shared_info->errorno == 0)
        {
            // SOCK_INFO indicates a m_socket call, proceed to create a UDP socket
            int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
            if (sockfd == -1)
            {
                // If error, update SOCK_INFO
                shared_info->sock_id = -1;
                shared_info->errorno = errno;
            }
            else
            {
                // If successful, update SOCK_INFO with the socket id
                shared_info->sock_id = sockfd;
                int optval = 1;
                if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
                {
                    perror("ERROR setting socket option");
                    exit(1);
                }
                printf("SOcket is created\n");
            }
        }
        else if (shared_info->port == 0 && shared_info->errorno == 0)
        {

            if (close(shared_info->sock_id) >= 0)
            {
                printf("Close sucess\n");
            }
            else
            {
                shared_info->sock_id = -1;
                shared_info->errorno = errno;
                perror("close");
            }
        }
        else if (shared_info->port != 0 && shared_info->errorno == 0)
        {

            // SOCK_INFO indicates a m_bind call, proceed to make a bind() call
            struct sockaddr_in addr;
            memset(&addr, 0, sizeof(addr));
            addr.sin_family = AF_INET;
            addr.sin_port = htons(shared_info->port);
            inet_aton(shared_info->IP, &addr.sin_addr);

            int result = bind(shared_info->sock_id, (struct sockaddr *)&addr, sizeof(addr));

            if (result == -1)
            {
                // If error, update SOCK_INFO
                shared_info->sock_id = -1;
                shared_info->errorno = errno;
            }
            else
            {
                printf("Bind Successfull\n");
            }
        }

        // Signal semaphore 2 to indicate completion
        semaphore_signal(semaphore2); // Perform signal operation
    }


  

    return 0;
}