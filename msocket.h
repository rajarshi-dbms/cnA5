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

extern pthread_mutex_t mutex; // Declare the mutex variable
#define SOCK_MTP 5
#define MAX_MESSAGE_SIZE 1024*8
#define MAX_MTP_SEND_BUFFER_SIZE 10*MAX_MESSAGE_SIZE
#define MAX_MTP_RECEIVE_BUFFER_SIZE 5*MAX_MESSAGE_SIZE
#define MAX_MTP_SOCKETS 25

typedef struct
{
    // Define your MTP header structure here
} MTPHeader;

typedef struct
{
    // Define the structure for sender and receiver windows
} Window;

typedef struct
{
    bool free;
    pid_t pid;
    int UDPsocID;
    char IP[20];
    int port;
    char sbuf[MAX_MTP_SEND_BUFFER_SIZE];
    char rbuf[MAX_MTP_RECEIVE_BUFFER_SIZE];
    // Window swnd;
    // Window rwnd;
    bool isClosed;

} MTPSocket;


typedef struct
{

} Shared_Memo;
int m_socket(int domain, int type, int protocol);
int m_bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
ssize_t m_sendto(int sockfd, const void *buf, size_t len, int flags, const struct sockaddr *dest_addr, socklen_t addrlen);
ssize_t m_recvfrom(int sockfd, void *buf, size_t len, int flags, struct sockaddr *src_addr, socklen_t *addrlen);
int m_close(int sockfd);

#endif
