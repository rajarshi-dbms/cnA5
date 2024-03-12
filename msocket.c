#include "msocket.h"

// Implement MTP functions here

int dropMessage(float p)
{
    // Simulate message dropping based on probability p
    // Return 1 if the message is dropped, 0 otherwise
}

// Implement other MTP functions as specified in the assignment
int m_socket(int domain, int type, int flag){

    if (type != SOCK_MTP)
    {
        perror("socket type was wrong");
        exit(1);
    }
    
    pthread_mutex_lock(&mutex);
    
}