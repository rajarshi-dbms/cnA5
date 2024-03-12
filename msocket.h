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
#include <semaphore.h>

// Declare the two semaphores
extern sem_t semaphore1;
extern sem_t semaphore2;


#define SOCK_MTP 5
#define MAX_MESSAGE_SIZE 1024*8
#define MAX_MTP_SEND_BUFFER_SIZE 10*MAX_MESSAGE_SIZE
#define MAX_MTP_RECEIVE_BUFFER_SIZE 5*MAX_MESSAGE_SIZE
#define MAX_MTP_SOCKETS 25

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <errno.h>

#define SHM_KEY_PATH "/tmp" 
#define SHM_KEY_ID 'S'      

typedef struct {
    int sock_id;
    char IP[256];  
    int port;
    int error_code;
} SOCK_INFO ;



int m_socket(int domain, int type, int protocol);

int m_bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
int m_sendto(int sockfd, const void *buf, size_t len, int flags, const struct sockaddr *dest_addr, socklen_t addrlen);
int m_recvfrom(int sockfd, void *buf, size_t len, int flags, struct sockaddr *src_addr, socklen_t *addrlen);
int m_close(int sockfd);

#endif