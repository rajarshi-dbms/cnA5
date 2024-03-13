#include <sys/socket.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <bits/pthreadtypes.h>

#ifndef MSOCKET_H
#define MSOCKET_H
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <errno.h>

#define SHM_KEY_PATH "/tmp"
#define SHM_KEY_ID 'S'
#define MTP_KEY_PATH "/tmp"
#define MTP_KEY_ID 'M'
#define SEM_KEY1_ID 'a'
#define SEM_KEY2_ID 'b'

#include <semaphore.h>

#define SOCK_MTP 5
#define MAX_MESSAGE_SIZE 1024 * 8
#define MAX_MTP_SEND_BUFFER_SIZE 10 * MAX_MESSAGE_SIZE
#define MAX_MTP_RECEIVE_BUFFER_SIZE 5 * MAX_MESSAGE_SIZE
#define MAX_MTP_SOCKETS 25

// typedef struct
// {
//     // Define your MTP header structure here
// } MTPHeader;
extern int msocket_errno;
typedef struct
{
    int sock_id;
    char IP[256];
    int port;
    int errorno;
} SOCK_INFO;

typedef struct
{
    bool free;
    pid_t pid;
    int UDPsocID;
    char des_IP[20];

    int des_port;
    char sbuf[MAX_MTP_SEND_BUFFER_SIZE];
    char rbuf[MAX_MTP_RECEIVE_BUFFER_SIZE];
    int source_IP[20];
    int source_port;

    // Window swnd;
    // Window rwnd;
    // bool isClosed;

} MTPSocket;

int m_socket(int domain, int type, int protocol);
int m_bind(int sockfd, char source_ip[], int source_port, char dest_ip[], int dest_port);
ssize_t m_sendto(int sockfd, const void *buf, size_t len, int flags, const struct sockaddr *dest_addr, socklen_t addrlen);
ssize_t m_recvfrom(int sockfd, void *buf, size_t len, int flags, struct sockaddr *src_addr, socklen_t *addrlen);
int m_close(int sockfd);
void semaphore_signal(int semaphore_id);
void semaphore_wait(int semaphore_id);
void semaphore_init(int semaphore_id, int initial_value);

#endif