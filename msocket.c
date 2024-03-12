#include "msocket.h"
int msocket_errno = 0;
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
    // Access and modify the shared data
    printf("sock_id: %d\n", shared_info->sock_id);
    printf("IP: %s\n", shared_info->IP);
    printf("port: %d\n", shared_info->port);
    printf("errorno: %d\n", shared_info->errorno);
    return shared_info;
}
// Function to initialize semaphore
void semaphore_init(int semaphore_id, int initial_value)
{
    union semun
    {
        int val;
        struct semid_ds *buf;
        unsigned short *array;
    } argument;
    argument.val = initial_value;
    if (semctl(semaphore_id, 0, SETVAL, argument) == -1)
    {
        perror("Semaphore initialization failed");
        exit(EXIT_FAILURE);
    }
}

// Function to perform semaphore wait (P) operation
void semaphore_wait(int semaphore_id)
{
    struct sembuf semaphore_operation;
    semaphore_operation.sem_num = 0;        // Index of semaphore in semaphore set
    semaphore_operation.sem_op = -1;        // Decrement semaphore value by 1
    semaphore_operation.sem_flg = SEM_UNDO; // Undo the operation if the process terminates
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
    semaphore_operation.sem_num = 0;        // Index of semaphore in semaphore set
    semaphore_operation.sem_op = 1;         // Increment semaphore value by 1
    semaphore_operation.sem_flg = SEM_UNDO; // Undo the operation if the process terminates
    if (semop(semaphore_id, &semaphore_operation, 1) == -1)
    {
        perror("Semaphore signal (V) operation failed");
        exit(EXIT_FAILURE);
    }
}
// Implement MTP functions here

int dropMessage(float p)
{
    // Simulate message dropping based on probability p
    // Return 1 if the message is dropped, 0 otherwise
}

// Implement other MTP functions as specified in the assignment
int m_socket(int domain, int type, int flag)
{

    if (type != SOCK_MTP)
    {
        perror("socket type was wrong");
        exit(1);
    }

    key_t key1;
    int shmid1;
    MTPSocket *sockM;

    // Generate the same key using ftok
    if ((key1 = ftok(MTP_KEY_PATH, MTP_KEY_ID)) == -1)
    {
        perror("ftok");
        exit(EXIT_FAILURE);
    }

    // Get the identifier for the shared memory segment
    if ((shmid1 = shmget(key1, sizeof(MTPSocket) * MAX_MTP_SOCKETS, 0666)) == -1)
    {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    // Attach the shared memory segment to our data structure
    if ((sockM = (MTPSocket *)shmat(shmid1, NULL, 0)) == (MTPSocket *)-1)
    {
        perror("shmat");
        exit(EXIT_FAILURE);
    }
    int i = 0;
    for (i = 0; i < MAX_MTP_SOCKETS; ++i)
    {
        if (sockM[i].free)
        {
            sockM[i].free = false;
            // signal on semaphore1 and wait on semaphore2

            break;
        }
    }
    if (i == MAX_MTP_SOCKETS)
    {
        msocket_errno = ENOBUFS; // Update the global error variable
        return -1;
    }

    SOCK_INFO *shared_info = shared_func();
    printf("Socket creation initiated...\n");
    key_t sem_key1, sem_key2;
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

    int semaphore1;
    if ((semaphore1 = semget(sem_key1, 1, 0666)) == -1)
    {
        perror("semget for semaphore1");
        exit(EXIT_FAILURE);
    }

    int semaphore2;
    if ((semaphore2 = semget(sem_key2, 1, 0666)) == -1)
    {
        perror("semget for semaphore1");
        exit(EXIT_FAILURE);
    }

    semaphore_signal(semaphore1); // Perform signal operation

    printf("Waiting for socket creation to complete...\n");

    semaphore_wait(semaphore2); // Perform wait operation

    printf("Socket creation completed.\n");

    // Check SOCK_INFO for error after completion
    int r = shared_info->sock_id;
    errno = shared_info->errorno;
    shared_info->sock_id = 0;
    strcpy(shared_info->IP, "");
    shared_info->port = 0;
    shared_info->errorno = 0;
    if (r == -1)
    {

        perror("Socket creation error");
        return -1;
    }
    else
    {
        printf("Socket created successfully: %d\n", shared_info->sock_id);
    }

    sockM[i].UDPsocID = r;
    sockM[i].pid = getpid();
    // Detach the shared memory segment
    if (shmdt(sockM) == -1)
    {
        perror("shmdt");
        exit(EXIT_FAILURE);
    }
    if (shmdt(shared_info) == -1)
    {
        perror("shmdt");
        exit(EXIT_FAILURE);
    }
    return i;
    // pthread_mutex_lock(&mutex);
}

int main()
{

    int sock_mtpfd = m_socket(AF_INET, SOCK_MTP, 0);

    printf("Created %d id\n", sock_mtpfd);
    sock_mtpfd = m_socket(AF_INET, SOCK_MTP, 0);

    printf("Created %d id\n", sock_mtpfd);

    return 0;
}