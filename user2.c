#include<stdio.h>
#include<stdlib.h>
#include<msocket.h>
#include<unistd.h>
#include<pthread.h>

int main()
{
    // Create MTP socket
    int sockfd = m_socket(AF_INET, SOCK_MTP, 0);
    if (sockfd < 0)
    {
        perror("m_socket");
        exit(1);
    }
    return 0;
}