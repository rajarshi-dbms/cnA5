#include <sys/socket.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <bits/pthreadtypes.h>
#include <signal.h>
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
#include <error.h>
#include <errno.h>

#define SHM_KEY_PATH "/tmp"
#define SHM_KEY_ID 'S'
#define MTP_KEY_PATH "/tmp"
#define MTP_KEY_ID 'M'
#define SEM_KEY1_ID 'a'
#define SEM_KEY2_ID 'b'
#define SEM_KEY3_ID 'c'
#define SEM_KEY4_ID 'd'
#include <semaphore.h>

#define T 2

#define SOCK_MTP 5
#define MAX_MESSAGE_SIZE 1024
#define MAX_MTP_SEND_BUFFER_SIZE 10 * MAX_MESSAGE_SIZE
#define MAX_MTP_RECEIVE_BUFFER_SIZE 5 * MAX_MESSAGE_SIZE
#define MAX_MTP_SOCKETS 25

// typedef struct
// {
//     // Define your MTP header structure here
// } MTPHeader;

// data structure for temporary information about a socket
typedef struct
{
    int sock_id;
    char IP[256];
    int port;
    int errorno;
} SOCK_INFO;

// data structure for shared memory of mtp
typedef struct
{
    /* Indicates whether the socket is free or not */
    bool free;
    /* Process ID of the process that has this socket */
    pid_t pid;
    /* UDP socket ID for sending MTP messages */
    int UDPsocID;
    /* Destination IP address for sending messages */
    char des_IP[20];
    /* Indicates whether there is no space in the send buffer */
    bool nospace;
    /* Indicates whether there is no space in the send buffer for sending acks*/
    bool sendNospace;
    /* Destination port for sending messages */
    int des_port;
    /* Send buffer for holding messages that are waiting to be sent */
    char sbuf[10][MAX_MESSAGE_SIZE];
    /* Receive buffer for holding messages that have been received */
    char rbuf[5][MAX_MESSAGE_SIZE];
    /* Source IP address */
    char source_IP[20];
    /* current size of the send buffer */
    int current_send;
    /* Source port */
    int source_port;
    /* Index of the slot where next msg will be written */
    int l_send;
    /* Index of the slot from where next msg will be read */
    int f_send;
    /* Ack number for the messages that have been received */
    int ack_num;
    

} MTPSocket;

int m_socket(int domain, int type, int protocol);
int m_bind(int sockfd, char source_ip[], int source_port, char dest_ip[], int dest_port);
int m_sendto(int sockfd, char buf[], int len);
int m_recvfrom(int sockfd, char buf[], int len);
int m_close(int sockfd);
void semaphore_signal(int semaphore_id);
void semaphore_wait(int semaphore_id);
void semaphore_init(int semaphore_id, int initial_value);

#endif