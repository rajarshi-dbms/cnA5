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
    // // Access and modify the shared data
    // printf("sock_id: %d\n", shared_info->sock_id);
    // printf("IP: %s\n", shared_info->IP);
    // printf("port: %d\n", shared_info->port);
    // printf("errorno: %d\n", shared_info->errorno);
    return shared_info;
}
// Function to initialize semaphore
void semaphore_init(int semaphore_id, int initial_value)
{
    // union semun
    // {
    //     int val;
    //     struct semid_ds *buf;
    //     unsigned short *array;
    // } argument;
    // argument.val = initial_value;
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

    return i;
    // pthread_mutex_lock(&mutex);
}

int m_bind(int sockfd, char source_ip[], int source_port, char dest_ip[], int dest_port)
{
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

    if (sockfd == -1 || sockfd >= MAX_MTP_SOCKETS || sockM[sockfd].free != 0 || sockM[sockfd].des_port != 0)
    {
        printf("Socket out of bound or not valid arguments or already bind\n");
        return -1;
    }

    SOCK_INFO *shared_info = shared_func();
    printf("Socket Binding initiated...\n");
    // setting ip and port in sock info
    shared_info->sock_id = sockM[sockfd].UDPsocID;
    strcpy(shared_info->IP, source_ip);
    shared_info->port = source_port;

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

    printf("Waiting for socket binding to complete...\n");

    semaphore_wait(semaphore2); // Perform wait operation

    int r = shared_info->sock_id;
    errno = shared_info->errorno;
    shared_info->sock_id = 0;
    strcpy(shared_info->IP, "");
    shared_info->port = 0;
    shared_info->errorno = 0;
    if (r == -1)
    {
        perror("Binding Fail\n");
       
        return -1;
    }
    else
    {
        sockM[sockfd].des_port = dest_port;
        strcpy(sockM[sockfd].des_IP, dest_ip);

        printf("Socket created successfully \n");
    }
    
    return 1;
}

int m_close(int sockfd)
{
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

    if (sockfd == -1 || sockfd >= MAX_MTP_SOCKETS || sockM[sockfd].free != 0 )
    {
        printf("Invalid socket\n");
        return -1;
    }
        int i=sockfd;
        sockM[i].free = true;
        sockM[i].pid = 0;
        sockM[i].UDPsocID = -1;
        memset(sockM[i].des_IP, 0, sizeof(sockM[i].des_IP));
        sockM[i].des_port = 0;
        memset(sockM[i].source_IP, 0, sizeof(sockM[i].source_IP));
        sockM[i].source_port = 0;
        memset(sockM[i].sbuf, 0, sizeof(sockM[i].sbuf));
        memset(sockM[i].rbuf, 0, sizeof(sockM[i].rbuf));

    close(sockM[sockfd].UDPsocID);
    printf("success closed \n");
    return 1;


}

int main(int argc, char *argv[])
{

    int sock_mtpfd = m_socket(AF_INET, SOCK_MTP, 0);

    printf("Created %d id\n", sock_mtpfd);
    // sock_mtpfd = m_socket(AF_INET, SOCK_MTP, 0);

    

    if (m_bind(sock_mtpfd, "loopback", atoi(argv[1]), "loopback", 20001) > 0)
    {
        printf("success\n");
    }
    m_close(sock_mtpfd);


    return 0;
}